using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.Json;

namespace BrailleApp.Services
{
    public static class BrailleKeymapLoader
    {
        private static readonly JsonSerializerOptions JsonOptions = new JsonSerializerOptions
        {
            PropertyNameCaseInsensitive = true
        };

        public static BrailleKeymap? LoadForIdentifier(string identifier)
        {
            if (string.IsNullOrWhiteSpace(identifier))
                return null;

            identifier = identifier.ToUpperInvariant();

            // Directory where you put your keymaps
            var baseDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "keymaps");

            //

            foreach (var file in Directory.EnumerateFiles(baseDir, "*.json"))
            {
                System.Diagnostics.Debug.WriteLine($"[LOADER] Found file: {file}");

                string json;
                try
                {
                    json = File.ReadAllText(file);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine($"[LOADER] Cannot read file: {file} → {ex.Message}");
                    continue;
                }


                BrailleKeymap? km;
                try
                {
                    km = JsonSerializer.Deserialize<BrailleKeymap>(json, JsonOptions);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine($"[LOADER] Invalid JSON in file: {file} → {ex.Message}");
                    continue;
                }


                if (km == null)
                {
                    System.Diagnostics.Debug.WriteLine($"[LOADER] JSON returned null for file: {file}");
                    continue;
                }

                if (km.IdentifierPrefixes == null || km.IdentifierPrefixes.Count == 0)
                {
                    System.Diagnostics.Debug.WriteLine($"[LOADER] No identifierPrefixes in file: {file}");
                    continue;
                }

                // Logging the prefixes
                System.Diagnostics.Debug.WriteLine(
                    $"[LOADER] Checking prefixes in {Path.GetFileName(file)}: " +
                    $"{string.Join(", ", km.IdentifierPrefixes)}");

                if (km.IdentifierPrefixes.Any(p => identifier.StartsWith(p.ToUpperInvariant())))
                {
                    km.SourceFilePath = file;

                    System.Diagnostics.Debug.WriteLine(
                        $"[LOADER] → MATCH: {file} for identifier '{identifier}'");

                    return km;
                }
                else
                {
                    System.Diagnostics.Debug.WriteLine(
                        $"[LOADER] No match: identifier '{identifier}' does not match prefixes in {Path.GetFileName(file)}");
                }
            }



            //

            // 🔥 Add this for logging:
            System.Diagnostics.Debug.WriteLine(
                $"[LOADER] Looking for keymaps in folder: {baseDir}");

            if (!Directory.Exists(baseDir))
                return null;

            foreach (var file in Directory.EnumerateFiles(baseDir, "*.json"))
            {
                var json = File.ReadAllText(file);
                var km = JsonSerializer.Deserialize<BrailleKeymap>(json);

                if (km == null || km.IdentifierPrefixes == null || km.IdentifierPrefixes.Count == 0)
                    continue;

                if (km.IdentifierPrefixes.Any(p => identifier.StartsWith(p.ToUpperInvariant())))
                {
                    // NEW: store the source file path of the loaded keymap
                    km.SourceFilePath = file;
                    return km;
                }
            }


            return null;
        }
    }
}
