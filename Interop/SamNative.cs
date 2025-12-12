using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace SamInterop
{
    internal static class SamNative
    {
        // ======================================================
        // LOGGING
        // ======================================================

        public static bool EnableLogging { get; set; } = true;

        private static void Log(string message)
        {
            if (EnableLogging)
                Debug.WriteLine("[SAM] " + message);
        }

        // ======================================================
        // DLL SELECTION & CONSTANTS
        // ======================================================

        // CHANGE THIS TO "sam32.dll" if you are using the 32-bit SAM.
        private const string DllName = "sam64.dll";

        // From sam.h
        public const int DESCRIPTION_LEN = 128; // WCHAR[128]
        public const int IDENTIFIER_LEN = 16;  // char[16]

        // From sam.h: LANGID_UK_English 44 (Britain)
        public const uint LANGID_UK_English = 44;

        // For SamTranslateBraille
        public const ushort SAM_BRL_UNKNOWN_DOTS = 0;

        // WM_SAM message types (wParam & 0xFF)
        public const uint SAM_RESPOND = 1;
        public const uint SAM_LOST_SPEECH = 3;
        public const uint SAM_CONFIG_START = 4;
        public const uint SAM_CONFIG_END = 5;

        #region Enums and flags

        public enum SamError : uint
        {
            None = 0,
            InvalidAddress = 1,
            InvalidUnit = 2,
            InvalidHandle = 3,
            UnitOpen = 4,
            InvalidParamNumber = 5,
            InvalidLangId = 6,
            NoDescription = 7,
            InvalidValue = 8,
            InvalidVoiceNumber = 9,
            NoSam = 10,
            AlreadyOpen = 11,
            OpenFailed = 12,
            UnitFail = 13,
            InvalidFlags = 14,
            CantSpeakOrBraille = 15,
            InvalidVBlock = 16,
            UnitClosed = 17,
            NotInitialised = 18,
            InternalError = 19,
            NotASynth = 20,
            NotABraille = 21,
            WrongSamVersion = 22,
            InvalidStrip = 23,
            Reload = 24
        }

        public enum SamClientType : uint
        {
            ScreenReader = 1, // SAM_SR
            FullTalkingApp = 2, // SAM_FTAP
            PartialTalkingApp = 3, // SAM_PTAP
            BackgroundApp = 4  // SAM_BTAP
        }

        public enum SamControlCode : uint
        {
            Detect = 1,  // SAMCONTROL_DETECT
            NumSynth = 2,  // SAMCONTROL_NUM_SYNTH
            Config = 3,  // SAMCONTROL_CONFIG
            Sam = 4,  // SAMCONTROL_SAM
            DeviceSynth = 5,  // SAMCONTROL_DEVICE_SYNTH
            SelectSynth = 6,  // SAMCONTROL_SELECT_SYNTH
            OverrideOn = 7,  // SAMCONTROL_OVERRIDE_ON
            OverrideOff = 8,  // SAMCONTROL_OVERRIDE_OFF
            Quit = 9,  // SAMCONTROL_QUIT
            DeviceBraille = 10, // SAMCONTROL_DEVICE_BRAILLE
            NumBraille = 11, // SAMCONTROL_NUM_BRAILLE
            SetLang = 12, // SAMCONTROL_SETLANG
            SelectBraille = 13, // SAMCONTROL_SELECT_BRAILLE
            Licensing = 14  // SAMCONTROL_LICENSING
        }

        #endregion

        #region Structures

        /// <summary>
        /// BRAILLEPARAMS from sam.h (v1 braille info).
        /// </summary>
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct BRAILLEPARAMS
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DESCRIPTION_LEN)]
            public string Description;   // WCHAR description[128]

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = IDENTIFIER_LEN)]
            public byte[] Identifier;    // char identifier[16]

            public uint Width;           // Number of display cells along the line
            public uint Height;          // Number of lines of Braille cells
            public uint Status;          // Number of status cells
            public uint Buttons;         // Number of buttons
            public uint MaxRate;         // Maximum cursor blink rate
            public uint Caps;            // Unit capabilities
            public uint Langs;           // Available description languages
        }

        #endregion

        // ======================================================
        // PRIVATE RAW P/INVOKE FUNCTIONS
        // ======================================================

        [DllImport(DllName, EntryPoint = "SamInit", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamInit_Native(
            SamClientType type,
            IntPtr hwnd,
            ushort message
        );

        [DllImport(DllName, EntryPoint = "SamEnd", CallingConvention = CallingConvention.Winapi)]
        private static extern void SamEnd_Native();

        [DllImport(DllName, EntryPoint = "SamRespond", CallingConvention = CallingConvention.Winapi)]
        private static extern void SamRespond_Native();

        [DllImport(DllName, EntryPoint = "SamControl", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamControl_Native(
            SamControlCode code,
            uint unitId
        );

        [DllImport(DllName, EntryPoint = "SamQueryBraille", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamQueryBraille_Native(
            uint unitId,
            out BRAILLEPARAMS brailleParams,
            uint langId
        );

        [DllImport(DllName, EntryPoint = "SamOpenBraille", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamOpenBraille_Native(
            uint unitId,
            out IntPtr brailleHandle
        );

        [DllImport(DllName, EntryPoint = "SamCloseBraille", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamCloseBraille_Native(
            IntPtr brailleHandle
        );

        [DllImport(DllName, EntryPoint = "SamSetDisplay", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamSetDisplay_Native(
            IntPtr brailleHandle,
            [MarshalAs(UnmanagedType.LPArray)] ushort[] dispCells,
            [MarshalAs(UnmanagedType.LPArray)] ushort[] statCells
        );

        [DllImport(DllName, EntryPoint = "SamClearDisplay", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamClearDisplay_Native(
            IntPtr brailleHandle
        );

        [DllImport(DllName, EntryPoint = "SamSetCursor", CallingConvention = CallingConvention.Winapi)]
        private static extern uint SamSetCursor_Native(
            IntPtr brailleHandle,
            int pos,
            ushort shape,
            uint rate
        );

        [DllImport(DllName, EntryPoint = "SamTranslateBraille", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode)]
        private static extern uint SamTranslateBraille_Native(
            [MarshalAs(UnmanagedType.LPWStr)] string text,
            uint textLength,
            [MarshalAs(UnmanagedType.LPArray)] ushort[] braille,
            ushort unknownDots
        );

        // ======================================================
        // PUBLIC WRAPPERS WITH LOGGING
        // ======================================================

        public static uint SamInit(SamClientType type, IntPtr hwnd, ushort message)
        {
            Log($"Calling SamInit(type={type}, hwnd=0x{hwnd.ToInt64():X}, msg={message})");
            uint r = SamInit_Native(type, hwnd, message);
            Log($" → SamInit returned {r} ({(SamError)r})");
            return r;
        }

        public static void SamEnd()
        {
            Log("Calling SamEnd()");
            SamEnd_Native();
            Log(" → SamEnd completed");
        }

        public static void SamRespond()
        {
            Log("Calling SamRespond()");
            SamRespond_Native();
            Log(" → SamRespond completed");
        }

        /// <summary>
        /// General control function.
        /// NOTE: For NumBraille / NumSynth, the return value is a COUNT, not an error code.
        /// </summary>
        public static uint SamControl(SamControlCode code, uint unitId)
        {
            Log($"Calling SamControl(code={code}, unitId={unitId})");
            uint r = SamControl_Native(code, unitId);

            if (code == SamControlCode.NumBraille || code == SamControlCode.NumSynth)
            {
                Log($" → SamControl ({code}) returned count={r}");
            }
            else
            {
                Log($" → SamControl returned {r} ({(SamError)r})");
            }

            return r;
        }

        /// <summary>
        /// Query a braille unit (v1 API).
        /// </summary>
        public static uint SamQueryBraille(uint unitId, out BRAILLEPARAMS brailleParams, uint langId)
        {
            Log($"Calling SamQueryBraille(unitId={unitId}, langId={langId})");
            uint r = SamQueryBraille_Native(unitId, out brailleParams, langId);
            Log($" → SamQueryBraille returned {r} ({(SamError)r})");
            if (r == (uint)SamError.None)
            {
                Log($"   Device=\"{brailleParams.Description}\", {brailleParams.Width}x{brailleParams.Height}, " +
                    $"Status={brailleParams.Status}, Buttons={brailleParams.Buttons}, Langs={brailleParams.Langs}");
            }
            return r;
        }

        public static uint SamOpenBraille(uint unitId, out IntPtr brailleHandle)
        {
            Log($"Calling SamOpenBraille(unitId={unitId})");
            uint r = SamOpenBraille_Native(unitId, out brailleHandle);
            Log($" → SamOpenBraille returned {r} ({(SamError)r}), handle=0x{brailleHandle.ToInt64():X}");
            return r;
        }

        public static uint SamCloseBraille(IntPtr brailleHandle)
        {
            Log($"Calling SamCloseBraille(handle=0x{brailleHandle.ToInt64():X})");
            uint r = SamCloseBraille_Native(brailleHandle);
            Log($" → SamCloseBraille returned {r} ({(SamError)r})");
            return r;
        }

        public static uint SamSetDisplay(IntPtr brailleHandle, ushort[] dispCells, ushort[] statCells)
        {
            int dispLen = dispCells?.Length ?? 0;
            int statLen = statCells?.Length ?? 0;

            Log($"Calling SamSetDisplay(handle=0x{brailleHandle.ToInt64():X}, dispCells={dispLen}, statCells={statLen})");
            uint r = SamSetDisplay_Native(brailleHandle, dispCells, statCells);
            Log($" → SamSetDisplay returned {r} ({(SamError)r})");
            return r;
        }

        public static uint SamClearDisplay(IntPtr brailleHandle)
        {
            Log($"Calling SamClearDisplay(handle=0x{brailleHandle.ToInt64():X})");
            uint r = SamClearDisplay_Native(brailleHandle);
            Log($" → SamClearDisplay returned {r} ({(SamError)r})");
            return r;
        }

        public static uint SamSetCursor(IntPtr brailleHandle, int pos, ushort shape, uint rate)
        {
            Log($"Calling SamSetCursor(handle=0x{brailleHandle.ToInt64():X}, pos={pos}, shape={shape}, rate={rate})");
            uint r = SamSetCursor_Native(brailleHandle, pos, shape, rate);
            Log($" → SamSetCursor returned {r} ({(SamError)r})");
            return r;
        }

        public static uint SamTranslateBraille(string text, uint textLength, ushort[] braille, ushort unknownDots)
        {
            Log($"Calling SamTranslateBraille(text=\"{text}\", length={textLength})");
            uint r = SamTranslateBraille_Native(text, textLength, braille, unknownDots);
            Log($" → SamTranslateBraille returned {r} ({(SamError)r})");
            return r;
        }
    }
}
