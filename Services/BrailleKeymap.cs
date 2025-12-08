using System.Collections.Generic;

namespace BrailleApp.Services
{
    public sealed class BrailleKeymap
    {
        public string DeviceId { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;

        public List<string> IdentifierPrefixes { get; set; } = new();

        public int CursorRoutingStrip { get; set; } = 0;
        public int ThumbKeyStrip { get; set; } = 0;

        // NEW: from which JSON file did we load this?
        public string SourceFilePath { get; set; } = string.Empty;

        // buttonIndex -> eventName
        public Dictionary<int, string> ThumbKeys { get; set; } = new();

        // optional future extension
        public List<OtherKeyMapping>? OtherKeys { get; set; }
    }


    public sealed class OtherKeyMapping
    {
        public int Strip { get; set; }
        public int Button { get; set; }
        public string Event { get; set; } = string.Empty;
    }


}
