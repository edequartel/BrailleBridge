using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Input;
using BrailleApp.Services;
using System.Linq;

namespace BrailleApp.ViewModels
{
    public sealed class BrailleDisplayViewModel : INotifyPropertyChanged
    {
        private readonly SamBrailleDisplayService _brailleService;

        private string _inputText = "Hello braille!";      // live typing box
        private string _secondText = "This is my memo.";   // memo box (Send button)
        private string _status = "Not initialized";
        private string _brailleMonitorText = string.Empty;

        // Indicates that SAM is initialized and a braille unit is open
        private bool _brailleReady = false;

        /// <summary>
        /// Text currently (or last) shown on the braille display.
        /// Shown in the "braille monitor" at the top.
        /// </summary>
        public string BrailleMonitorText
        {
            get => _brailleMonitorText;
            set
            {
                if (value != _brailleMonitorText)
                {
                    _brailleMonitorText = value;
                    OnPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Live typing box – every change is sent automatically to the braille display.
        /// </summary>
        public string InputText
        {
            get => _inputText;
            set
            {
                if (value != _inputText)
                {
                    _inputText = value;
                    OnPropertyChanged();

                    if (_brailleReady)
                    {
                        var toShow = TruncateToDisplay(_inputText);
                        if (!_brailleService.ShowText(toShow, out var error))
                        {
                            Status = $"Error sending to braille: {error}";
                        }
                        else
                        {
                            BrailleMonitorText = toShow;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Memo box – only sent when pressing the Send button.
        /// </summary>
        public string SecondText
        {
            get => _secondText;
            set
            {
                if (value != _secondText)
                {
                    _secondText = value;
                    OnPropertyChanged();
                }
            }
        }

        //

        public void OnSamConfigStart()
        {
            Status = "SAM configuration is changing…";
        }

        public void OnSamConfigEnd()
        {
            Status = "SAM configuration finished (no refresh yet).";
            //RefreshUnits();
            Initialize();
        }

        //

        public string Status
        {
            get => _status;
            set
            {
                if (value != _status)
                {
                    _status = value;
                    OnPropertyChanged();
                }
            }
        }

        // List of "1: Focus 40", "2: Virtual on-screen display", etc.
        public ObservableCollection<string> AvailableUnits { get; } = new();

        private int _selectedUnitIndex;
        public int SelectedUnitIndex
        {
            get => _selectedUnitIndex;
            set
            {
                if (value != _selectedUnitIndex)
                {
                    _selectedUnitIndex = value;
                    OnPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Log of braille key/button events from WM_SAM.
        /// </summary>
        public ObservableCollection<string> BrailleKeyEvents { get; } = new();

        public ICommand InitializeCommand { get; }
        public ICommand SendCommand { get; }
        public ICommand ClearCommand { get; }
        public ICommand RefreshUnitsCommand { get; }

        public BrailleDisplayViewModel(SamBrailleDisplayService brailleService)
        {
            _brailleService = brailleService ?? throw new ArgumentNullException(nameof(brailleService));

            InitializeCommand = new RelayCommand(_ => Initialize());
            SendCommand = new RelayCommand(_ => Send());
            ClearCommand = new RelayCommand(_ => Clear());
            RefreshUnitsCommand = new RelayCommand(_ => RefreshUnits());
        }

        /// <summary>
        /// Initialise SAM and open the chosen braille unit.
        /// Called from the UI (button / auto-init in MainWindow).
        /// </summary>
        private void Initialize()
        {
            // 1. Initialise SAM
            if (!_brailleService.Initialize(out var initError))
            {
                Status = $"Init failed: {initError}";
                _brailleReady = false;
                return;
            }

            // 2. Get list of units
            if (!_brailleService.TryGetBrailleUnits(out var units, out var listError))
            {
                Status = $"Failed to get braille units: {listError}";
                _brailleReady = false;
                return;
            }

            AvailableUnits.Clear();
            foreach (var (unitId, desc) in units)
            {
                AvailableUnits.Add($"{unitId}: {desc}");
            }

            if (AvailableUnits.Count == 0)
            {
                Status = "No braille units found.";
                _brailleReady = false;
                return;
            }

            if (SelectedUnitIndex < 0 || SelectedUnitIndex >= AvailableUnits.Count)
                SelectedUnitIndex = 0;

            // 3. Open the selected unit (1-based id)
            uint unitIdToOpen = (uint)(SelectedUnitIndex + 1);

            if (_brailleService.OpenUnit(unitIdToOpen, out var openError))
            {
                Status = $"Connected to: {_brailleService.DeviceDescription} " +
                         $"({_brailleService.Width}×{_brailleService.Height})";
                _brailleReady = true;

                // Optional: show current live text immediately
                if (!string.IsNullOrEmpty(_inputText))
                {
                    var toShow = TruncateToDisplay(_inputText);
                    _brailleService.ShowText(toShow, out _);
                    BrailleMonitorText = toShow;
                }
            }
            else
            {
                Status = $"Open failed for unit {unitIdToOpen}: {openError}";
                _brailleReady = false;
            }
        }

        private void RefreshUnits()
        {
            if (!_brailleService.Initialize(out var initError))
            {
                Status = $"Init failed: {initError}";
                _brailleReady = false;
                return;
            }

            if (_brailleService.TryGetBrailleUnits(out var units, out var error))
            {
                AvailableUnits.Clear();
                foreach (var (unitId, desc) in units)
                    AvailableUnits.Add($"{unitId}: {desc}");

                Status = AvailableUnits.Count == 0
                    ? "No braille units found."
                    : "Units refreshed.";
            }
            else
            {
                Status = $"Failed to get braille units: {error}";
            }
        }

        /// <summary>
        /// Send the content of the memo box (SecondText) to the braille display.
        /// Called from the UI Send button.
        /// </summary>
        private void Send()
        {
            var text = SecondText ?? string.Empty;
            var toShow = TruncateToDisplay(text);

            if (_brailleService.ShowText(toShow, out var error))
            {
                Status = $"Sent memo to braille: \"{text}\"";
                BrailleMonitorText = toShow;
            }
            else
            {
                Status = $"Error sending memo to braille: {error}";
            }
        }

        public void ClearFromAnyThread()
        {
            // If we're already on the UI thread, just call Clear()
            if (Application.Current.Dispatcher.CheckAccess())
            {
                Clear();
            }
            else
            {
                // Marshal to UI thread so Status and BrailleMonitorText update safely
                Application.Current.Dispatcher.Invoke(Clear);
            }
        }

        private void Clear()
        {
            if (_brailleService.Clear(out var error))
            {
                Status = "Display cleared";
                BrailleMonitorText = string.Empty;
            }
            else
            {
                Status = $"Error clearing display: {error}";
            }
        }

        /// <summary>
        /// Called from MainWindow WndProc when a WM_SAM braille button message arrives.
        /// (Already on UI thread, so no dispatcher needed here.)
        /// </summary>
        public void AddBrailleKeyEvent(uint msgType, uint unitId, uint strip, uint param)
        {
            string typeName = msgType switch
            {
                8 => "NORMAL",
                9 => "CSRDISP",
                10 => "CSRSTAT",
                11 => "AUXDISP",
                12 => "AUXSTAT",
                13 => "KEYPAD",
                14 => "STRIP",
                _ => msgType.ToString()
            };

            string text =
                $"{DateTime.Now:HH:mm:ss}  Unit={unitId} Strip={strip} Type={typeName} Param=0x{param:X8}";

            // Insert at top
            BrailleKeyEvents.Insert(0, text);

            // Keep list from getting huge
            if (BrailleKeyEvents.Count > 200)
            {
                BrailleKeyEvents.RemoveAt(BrailleKeyEvents.Count - 1);
            }
        }

        /// <summary>
        /// NEW:
        /// Safely show text on the braille display from ANY thread (UI or background).
        /// Use this from your HTTP/WebSocket bridge instead of calling _brailleService.ShowText directly.
        /// </summary>
        /// <param name="text">Text to send to the braille display.</param>
        public void ShowTextFromAnyThread(string text)
        {
            // Normalize null
            text ??= string.Empty;

            // Try to get the dispatcher (might be null during app shutdown)
            var dispatcher = Application.Current?.Dispatcher;

            if (dispatcher == null)
            {
                // No dispatcher available; nothing we can safely do.
                return;
            }

            // If we're already on the UI thread, call directly.
            if (dispatcher.CheckAccess())
            {
                ShowTextInternal(text);
            }
            else
            {
                // Hop back to UI thread
                dispatcher.BeginInvoke(new Action(() =>
                {
                    ShowTextInternal(text);
                }));
            }
        }

        /// <summary>
        /// Safely get a snapshot of the available braille units
        /// from ANY thread as a simple string array.
        /// Uses the WPF Application dispatcher internally.
        /// </summary>
        public string[] GetAvailableUnitsSnapshot()
        {
            var dispatcher = Application.Current?.Dispatcher;
            if (dispatcher == null)
            {
                // App is shutting down or not yet ready
                return Array.Empty<string>();
            }

            if (dispatcher.CheckAccess())
            {
                // Already on UI thread
                return AvailableUnits.ToArray();
            }

            // Hop to UI thread to read ObservableCollection safely
            return dispatcher.Invoke(() => AvailableUnits.ToArray());
        }


        /// <summary>
        /// Internal helper that assumes we are ALREADY on the UI thread.
        /// Does the actual SAM/braille work.
        /// </summary>
        /// <param name="text">Text to send to the braille display.</param>
        private void ShowTextInternal(string text)
        {
            if (!_brailleReady)
            {
                Status = "Braille not ready (SAM not initialized or unit not opened).";
                return;
            }

            var toShow = TruncateToDisplay(text);

            if (_brailleService.ShowText(toShow, out var error))
            {
                Status = $"Sent from bridge: \"{toShow}\"";
                BrailleMonitorText = toShow;
                // Also update InputText if you want the UI box to reflect last sent text:
                // InputText = toShow;  // optional
            }
            else
            {
                Status = $"Error sending from bridge: {error}";
            }
        }

        /// <summary>
        /// Ensure we don't show more characters than the braille display can show (Width × Height).
        /// </summary>
        private string TruncateToDisplay(string text)
        {
            if (string.IsNullOrEmpty(text))
                return string.Empty;

            uint cells = _brailleService.Width * _brailleService.Height;
            if (cells == 0)
                return text; // if we don't know, just show full

            if (text.Length > cells)
                return text.Substring(0, (int)cells);

            return text;
        }

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler? PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string? propertyName = null)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        #endregion
    }
}
