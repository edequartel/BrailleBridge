using System;
using System.Collections.Generic;
using System.Text;
using SamInterop;

namespace BrailleApp.Services
{
    /// <summary>
    /// High-level wrapper around SAM braille functions.
    /// - Initializes SAM
    /// - Can list all braille units
    /// - Can open a specific braille unit (1..NumBraille)
    /// - Provides methods to show text and clear the display
    /// </summary>
    public sealed class SamBrailleDisplayService : IDisposable
    {
        private readonly IntPtr _hwnd;
        private readonly ushort _samMessage;

        private bool _initialized;
        private bool _opened;
        private IntPtr _brlHandle = IntPtr.Zero;
        private uint _currentUnitId = 0;
        private uint _numBraille = 0;

        public uint Width { get; private set; }
        public uint Height { get; private set; }
        public string DeviceDescription { get; private set; } = string.Empty;
        public string DeviceIdentifier { get; private set; } = string.Empty;

        public SamBrailleDisplayService(IntPtr hwnd, uint samMessageId)
        {
            _hwnd = hwnd;
            _samMessage = (ushort)samMessageId;
        }

        /// <summary>
        /// Initialise SAM (but do NOT open a braille unit yet).
        /// Returns true on success, false on failure with 'error' filled.
        /// </summary>
        public bool Initialize(out string error)
        {
            error = string.Empty;

            if (_initialized)
            {
                return true;
            }

            // 1. SamInit – now explicitly as SCREEN READER
            uint res = SamNative.SamInit(SamNative.SamClientType.ScreenReader, _hwnd, _samMessage);
            if (res != (uint)SamNative.SamError.None)
            {
                error = $"SamInit failed: {res} ({(SamNative.SamError)res})";
                return false;
            }

            _initialized = true;

            // 2. Detect devices
            SamNative.SamControl(SamNative.SamControlCode.Detect, 0);

            // 3. Get number of braille units
            _numBraille = SamNative.SamControl(SamNative.SamControlCode.NumBraille, 0);
            if (_numBraille == 0)
            {
                error = "No braille units found via SAM (NumBraille returned 0).";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Get a list of all braille units (id + description).
        /// Returns true on success.
        /// </summary>
        public bool TryGetBrailleUnits(out List<(uint UnitId, string Description)> units, out string error)
        {
            units = new List<(uint, string)>();
            error = string.Empty;

            if (!_initialized)
            {
                error = "SAM not initialized. Call Initialize() first.";
                return false;
            }

            if (_numBraille == 0)
            {
                _numBraille = SamNative.SamControl(SamNative.SamControlCode.NumBraille, 0);
                if (_numBraille == 0)
                {
                    error = "No braille units found.";
                    return false;
                }
            }

            for (uint unitId = 1; unitId <= _numBraille; unitId++)
            {
                uint res = SamNative.SamQueryBraille(unitId, out var bp, SamNative.LANGID_UK_English);
                if (res == (uint)SamNative.SamError.None)
                {
                    units.Add((unitId, bp.Description ?? $"Unit {unitId}"));
                }
                else
                {
                    units.Add((unitId, $"Unit {unitId} (error {res})"));
                }
            }

            return true;
        }

        /// <summary>
        /// Open the specified braille unit (1..NumBraille).
        /// </summary>
        public bool OpenUnit(uint unitId, out string error)
        {
            error = string.Empty;

            if (!_initialized)
            {
                error = "SAM not initialized. Call Initialize() first.";
                return false;
            }

            if (unitId == 0)
            {
                error = "Unit ID must be >= 1.";
                return false;
            }

            // Close previous unit if open
            if (_opened && _brlHandle != IntPtr.Zero)
            {
                SamNative.SamCloseBraille(_brlHandle);
                _brlHandle = IntPtr.Zero;
                _opened = false;
            }

            // Query braille params for this unit with langId = 44 (UK English)
            uint res = SamNative.SamQueryBraille(unitId, out var bp, SamNative.LANGID_UK_English);
            if (res != (uint)SamNative.SamError.None)
            {
                error = $"SamQueryBraille failed for unitId={unitId}: {res} ({(SamNative.SamError)res})";
                return false;
            }

            // Open braille handle
            res = SamNative.SamOpenBraille(unitId, out _brlHandle);
            if (res != (uint)SamNative.SamError.None || _brlHandle == IntPtr.Zero)
            {
                error = $"SamOpenBraille failed for unitId={unitId}: {res} ({(SamNative.SamError)res})";
                return false;
            }

            _opened = true;
            _currentUnitId = unitId;

            // Identifier uit BRAILLEPARAMS omzetten naar string (ASCII, tot eerste 0-byte)
            if (bp.Identifier != null && bp.Identifier.Length > 0)
            {
                int len = Array.IndexOf(bp.Identifier, (byte)0);
                if (len < 0) len = bp.Identifier.Length;

                DeviceIdentifier = Encoding.ASCII.GetString(bp.Identifier, 0, len);
            }
            else
            {
                DeviceIdentifier = string.Empty;
            }


            Width = bp.Width;
            Height = bp.Height;
            DeviceDescription = bp.Description ?? $"Unit {unitId}";

            return true;
        }

        /// <summary>
        /// Show text on the braille display (simple v1 API).
        /// </summary>
        public bool ShowText(string text, out string error)
        {
            error = string.Empty;

            // Extra safety: check SAM + handle state
            if (!_initialized || !_opened || _brlHandle == IntPtr.Zero)
            {
                error = "SAM not initialized or braille unit not opened.";
                return false;
            }

            text ??= string.Empty;

            // Determine how many display cells we actually have
            uint totalCells = Width * Height;
            if (totalCells == 0)
            {
                error = "Braille width/height is 0; SamQueryBraille may have failed.";
                return false;
            }

            // Do not send more characters than fit on the display
            if (text.Length > totalCells)
                text = text.Substring(0, (int)totalCells);

            // 1) Translate text → braille cells (one per character for now)
            var braille = new ushort[text.Length];
            uint res = SamNative.SamTranslateBraille(
                text,
                (uint)text.Length,
                braille,
                SamNative.SAM_BRL_UNKNOWN_DOTS
            );

            if (res != (uint)SamNative.SamError.None)
            {
                error = $"SamTranslateBraille failed: {res} ({(SamNative.SamError)res})";
                return false;
            }

            // 2) Allocate FULL display buffer: Width × Height cells
            var dispCells = new ushort[totalCells];
            Array.Clear(dispCells, 0, dispCells.Length);

            // 3) Copy translated braille into the start of that buffer
            if (braille.Length > 0)
                Array.Copy(braille, dispCells, braille.Length);

            // 4) No explicit status cells → let SAM handle default status area
            res = SamNative.SamSetDisplay(_brlHandle, dispCells, null);
            if (res != (uint)SamNative.SamError.None)
            {
                error = $"SamSetDisplay failed: {res} ({(SamNative.SamError)res})";
                return false;
            }

            // Optional: put cursor at start of line
            SamNative.SamSetCursor(_brlHandle, 0, 0, 0);

            return true;
        }



        public bool Clear(out string error)
        {
            error = string.Empty;

            if (!_opened || _brlHandle == IntPtr.Zero)
            {
                error = "Braille unit not opened.";
                return false;
            }

            uint res = SamNative.SamClearDisplay(_brlHandle);
            if (res != (uint)SamNative.SamError.None)
            {
                error = $"SamClearDisplay failed: {res} ({(SamNative.SamError)res})";
                return false;
            }

            return true;
        }

        // QQQ>



        // QQQ<

        public void Dispose()
        {
            if (_opened && _brlHandle != IntPtr.Zero)
            {
                SamNative.SamCloseBraille(_brlHandle);
                _brlHandle = IntPtr.Zero;
                _opened = false;
            }

            if (_initialized)
            {
                SamNative.SamEnd();
                _initialized = false;
            }
        }
    }
}
