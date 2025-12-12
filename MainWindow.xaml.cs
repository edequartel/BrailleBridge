using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using BrailleApp.Services;
using BrailleApp.ViewModels;
using SamInterop;

namespace BrailleApp
{
    public partial class MainWindow : Window
    {
        // Moet overeenkomen met de naam die SAM gebruikt voor het window message
        private const string SamMessageName = "SAM_MESSAGE";

        private uint _samMessageId;
        private HwndSource? _hwndSource;

        private SamBrailleDisplayService? _brailleService;
        private BrailleWebSocketBridge? _bridge;

        public MainWindow()
        {
            InitializeComponent();
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);

            var helper = new WindowInteropHelper(this);
            IntPtr hwnd = helper.Handle;

            // SAM window message registreren
            _samMessageId = RegisterWindowMessage(SamMessageName);

            // WndProc hooken
            _hwndSource = HwndSource.FromHwnd(hwnd);
            _hwndSource.AddHook(WndProc);

            // SAM braille service aanmaken
            _brailleService = new SamBrailleDisplayService(hwnd, (ushort)_samMessageId);

            // ViewModel aanmaken (textbox, knoppen, status, etc.)
            var vm = new BrailleDisplayViewModel(_brailleService);
            DataContext = vm;

            // Bridge aanmaken (bijv. poort 5000), nu met ViewModel.
            _bridge = new BrailleWebSocketBridge(
                vm,
                port: 5000);
            _bridge.Start();

            // Automatisch SAM initialiseren en eerste braille-unit openen
            if (vm.InitializeCommand.CanExecute(null))
            {
                vm.InitializeCommand.Execute(null);
            }

            // Automatisch SAM initialiseren en eerste braille-unit openen
            if (vm.InitializeCommand.CanExecute(null))
            {
                vm.InitializeCommand.Execute(null);
            }

            // Na openen van de braille-unit: device identifier → JSON keymap → bridge
            if (_brailleService != null && _bridge != null)
            {
                System.Diagnostics.Debug.WriteLine(
                    $"[MAIN] DeviceIdentifier = '{_brailleService.DeviceIdentifier}'");

                var identifier = _brailleService.DeviceIdentifier;

                if (!string.IsNullOrWhiteSpace(identifier))
                {
                    var keymap = BrailleKeymapLoader.LoadForIdentifier(identifier);

                    if (keymap == null)
                    {
                        System.Diagnostics.Debug.WriteLine(
                            $"[MAIN] No keymap found for identifier '{identifier}'");
                    }
                    else
                    {
                        System.Diagnostics.Debug.WriteLine(
                            $"[MAIN] Loaded keymap DeviceId='{keymap.DeviceId}', file='{keymap.SourceFilePath}'");

                        _bridge.SetKeymap(keymap);
                    }
                }
                else
                {
                    System.Diagnostics.Debug.WriteLine("[MAIN] DeviceIdentifier is empty");
                }
            }


            // Na openen van de braille-unit: device identifier → JSON keymap → bridge
            if (_brailleService != null && _bridge != null)
            {
                var identifier = _brailleService.DeviceIdentifier;

                if (!string.IsNullOrWhiteSpace(identifier))
                {
                    var keymap = BrailleKeymapLoader.LoadForIdentifier(identifier);
                    _bridge.SetKeymap(keymap);
                }
            }

        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            // Bridge netjes opruimen
            _bridge?.Dispose();
            _bridge = null;

            // SAM afsluiten
            if (_brailleService is IDisposable d)
            {
                d.Dispose();
                _brailleService = null;
            }

            if (_hwndSource != null)
            {
                _hwndSource.RemoveHook(WndProc);
                _hwndSource = null;
            }
        }

        #region Win32 interop

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        private static extern uint RegisterWindowMessage(string lpString);

        private IntPtr WndProc(
            IntPtr hwnd,
            int msg,
            IntPtr wParam,
            IntPtr lParam,
            ref bool handled)
        {
            if ((uint)msg == _samMessageId)
            {
                HandleSamMessage(wParam, lParam);
                handled = true;
            }

            return IntPtr.Zero;
        }

        /// <summary>
        /// Uitpakken van de WM_SAM braille-button boodschap en doorgeven
        /// aan ViewModel + WebSocket bridge.
        /// </summary>
        //private void HandleSamMessage(IntPtr wParam, IntPtr lParam)
        //{
        //    uint w = unchecked((uint)wParam.ToInt64());
        //    uint l = unchecked((uint)lParam.ToInt64());

        //    uint msgType = w & 0xFF;         // GET_SAM_MSG
        //    uint unitId = (w >> 8) & 0xFF;   // GET_SAM_UNIT
        //    uint strip = (w >> 16) & 0xFF;   // GET_SAM_STRIP

        //    // Naar ViewModel loggen
        //    if (DataContext is BrailleDisplayViewModel vm)
        //    {
        //        vm.AddBrailleKeyEvent(msgType, unitId, strip, l);
        //    }

        //    // Naar WebSocket bridge sturen → HTML pagina krijgt events
        //    //if (_bridge != null)
        //    //{
        //    //    _ = _bridge.BroadcastBrailleKeyEventAsync(msgType, unitId, strip, l);
        //    //}


        //    //
        //    // Naar WebSocket bridge sturen → via keymap handler
        //    if (_bridge != null)
        //    {
        //        _bridge.HandleRawKeyFromSam(msgType, unitId, strip, l);
        //    }
        ////}


        //private void HandleSamMessage(IntPtr wParam, IntPtr lParam)
        //{
        //    uint w = unchecked((uint)wParam.ToInt64());
        //    uint l = unchecked((uint)lParam.ToInt64());

        //    uint msgType = w & 0xFF;         // GET_SAM_MSG
        //    uint unitId = (w >> 8) & 0xFF;  // GET_SAM_UNIT
        //    uint strip = (w >> 16) & 0xFF; // GET_SAM_STRIP

        //    System.Diagnostics.Debug.WriteLine(
        //        $"[WM_SAM] msgType={msgType}, unit={unitId}, strip={strip}, param=0x{l:X8}");

        //    // existing code:
        //    if (DataContext is BrailleDisplayViewModel vm)
        //    {
        //        vm.AddBrailleKeyEvent(msgType, unitId, strip, l);
        //    }

        //    if (_bridge != null)
        //    {
        //        _bridge.HandleRawKeyFromSam(msgType, unitId, strip, l);
        //    }
        //}

        private void HandleSamMessage(IntPtr wParam, IntPtr lParam)
        {
            uint w = unchecked((uint)wParam.ToInt64());
            uint l = unchecked((uint)lParam.ToInt64());

            uint msgType = w & 0xFF;
            uint unitId = (w >> 8) & 0xFF;
            uint strip = (w >> 16) & 0xFF;

            System.Diagnostics.Debug.WriteLine(
                $"[WM_SAM] msgType={msgType}, unit={unitId}, strip={strip}, param=0x{l:X8}");

            if (DataContext is BrailleDisplayViewModel vm)
            {
                // 1) Config start/end
                if (msgType == SamNative.SAM_CONFIG_START)
                {
                    vm.OnSamConfigStart();
                }
                else if (msgType == SamNative.SAM_CONFIG_END)
                {
                    vm.OnSamConfigEnd();
                }

                // 2) Existing logging of button events
                vm.AddBrailleKeyEvent(msgType, unitId, strip, l);
            }

            // 3) Existing bridge mapping (for key presses etc.)
            _bridge?.HandleRawKeyFromSam(msgType, unitId, strip, l);
        }

        #endregion
    }
}
