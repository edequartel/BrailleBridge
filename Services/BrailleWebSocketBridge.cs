using System;
using System.Collections.Concurrent;
using System.IO;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;
using BrailleApp.ViewModels;
using System.Diagnostics;



namespace BrailleApp.Services
{
    /// <summary>
    /// Bridge:
    /// - HTTP POST /braille: tekst naar brailleleesregel
    /// - WebSocket /ws: braille key events naar de browser pushen
    ///
    /// Alle SAM-calls lopen via BrailleDisplayViewModel.ShowTextFromAnyThread(),
    /// dus dezelfde route als de textbox in de UI.
    /// </summary>
    public sealed class BrailleWebSocketBridge : IDisposable
    {
        private readonly BrailleDisplayViewModel _viewModel;

        private readonly HttpListener _listener = new HttpListener();
        private readonly ConcurrentBag<WebSocket> _sockets = new();
        private readonly CancellationTokenSource _cts = new();

        private JsonKeymapMapper? _keymapMapper;

        public int Port { get; }

        private static void Log(string message)
        {
            Debug.WriteLine("[BRIDGE] " + message);
        }


        public BrailleWebSocketBridge(
            BrailleDisplayViewModel viewModel,
            int port = 5000)
        {
            _viewModel = viewModel ?? throw new ArgumentNullException(nameof(viewModel));
            Port = port;

            // Alleen localhost
            _listener.Prefixes.Add($"http://localhost:{port}/");
        }

        public void Start()
        {
            _listener.Start();
            _ = Task.Run(() => AcceptLoopAsync(_cts.Token));
        }

        private async Task AcceptLoopAsync(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                HttpListenerContext ctx;
                try
                {
                    ctx = await _listener.GetContextAsync().ConfigureAwait(false);
                }
                catch
                {
                    if (token.IsCancellationRequested)
                        break;
                    continue;
                }

                _ = Task.Run(() => HandleContextAsync(ctx, token), token);
            }
        }

        private async Task HandleContextAsync(HttpListenerContext ctx, CancellationToken token)
        {
            try
            {
                // CORS, zodat een HTML pagina (file:// of andere origin) mag praten met localhost
                ctx.Response.Headers.Add("Access-Control-Allow-Origin", "*");
                ctx.Response.Headers.Add("Access-Control-Allow-Headers", "Content-Type");
                ctx.Response.Headers.Add("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

                if (ctx.Request.HttpMethod == "OPTIONS")
                {
                    ctx.Response.StatusCode = 200;
                    ctx.Response.Close();
                    return;
                }

                var path = ctx.Request.Url?.AbsolutePath ?? "/";

                // WebSocket endpoint: ws://localhost:<port>/ws
                if (ctx.Request.IsWebSocketRequest && path == "/ws")
                {
                    var wsContext = await ctx.AcceptWebSocketAsync(subProtocol: null);
                    var socket = wsContext.WebSocket;
                    _sockets.Add(socket);

                    await ReceiveLoopAsync(socket, token);
                    return;
                }

                // HTTP POST /braille – body = plain text naar de brailleleesregel
                if (ctx.Request.HttpMethod == "POST" && path == "/braille")
                {
                    using var reader = new StreamReader(ctx.Request.InputStream, ctx.Request.ContentEncoding);
                    var body = await reader.ReadToEndAsync();

                    // 🔑 Gebruik dezelfde route als de textbox:
                    // ViewModel → ShowTextFromAnyThread → ShowTextInternal → SamBrailleDisplayService.ShowText
                    _viewModel.ShowTextFromAnyThread(body);

                    var okBytes = Encoding.UTF8.GetBytes("text received");
                    ctx.Response.StatusCode = 200;
                    ctx.Response.ContentType = "text/plain; charset=utf-8";
                    ctx.Response.ContentLength64 = okBytes.Length;
                    await ctx.Response.OutputStream.WriteAsync(okBytes, 0, okBytes.Length, token);
                    ctx.Response.Close();
                    return;
                }

                // HTTP GET /clear
                if (ctx.Request.HttpMethod == "GET" && path == "/clear")
                {
                    _viewModel.ClearFromAnyThread();


                    var okBytes = Encoding.UTF8.GetBytes("/clear received");
                    ctx.Response.StatusCode = 200;
                    ctx.Response.ContentType = "text/plain; charset=utf-8";
                    ctx.Response.ContentLength64 = okBytes.Length;
                    await ctx.Response.OutputStream.WriteAsync(okBytes, 0, okBytes.Length, token);
                    ctx.Response.Close();
                    return;

                }

                // HTTP GET /ping
                if (ctx.Request.HttpMethod == "GET" && path == "/ping")
                {
                    var okBytes = Encoding.UTF8.GetBytes("/ping received");
                    ctx.Response.StatusCode = 200;
                    ctx.Response.ContentType = "text/plain; charset=utf-8";
                    ctx.Response.ContentLength64 = okBytes.Length;
                    await ctx.Response.OutputStream.WriteAsync(okBytes, 0, okBytes.Length, token);
                    ctx.Response.Close();
                    return;

                }

                // HTTP GET /braille/devices – lijst met brailledisplays als JSON
                if (ctx.Request.HttpMethod == "GET" && path == "/devices")
                {
                    // Haal snapshot uit de ViewModel (thread-safe)
                    var units = _viewModel.GetAvailableUnitsSnapshot();

                    var json = JsonSerializer.Serialize(units);
                    var bytes = Encoding.UTF8.GetBytes(json);

                    ctx.Response.StatusCode = 200;
                    ctx.Response.ContentType = "application/json; charset=utf-8";
                    ctx.Response.ContentLength64 = bytes.Length;
                    await ctx.Response.OutputStream.WriteAsync(bytes, 0, bytes.Length, token);
                    ctx.Response.Close();
                    return;
                }


                // HTTP GET /braille/devices – lijst van braille-units als JSON
                if (ctx.Request.HttpMethod == "GET" && path == "/braille/test")
                {
                    var okBytes = Encoding.UTF8.GetBytes("/braille/test");
                    ctx.Response.StatusCode = 200;
                    ctx.Response.ContentType = "text/plain; charset=utf-8";
                    ctx.Response.ContentLength64 = okBytes.Length;
                    await ctx.Response.OutputStream.WriteAsync(okBytes, 0, okBytes.Length, token);
                    ctx.Response.Close();
                    return;

                }


                // Onbekende endpoint
                ctx.Response.StatusCode = 404;
                ctx.Response.Close();
            }
            catch
            {
                try
                {
                    ctx.Response.StatusCode = 500;
                    ctx.Response.Close();
                }
                catch
                {
                    // negeren
                }
            }
        }

        private async Task ReceiveLoopAsync(WebSocket socket, CancellationToken token)
        {
            var buffer = new byte[4096];

            try
            {
                while (socket.State == WebSocketState.Open && !token.IsCancellationRequested)
                {
                    var result = await socket.ReceiveAsync(new ArraySegment<byte>(buffer), token);

                    if (result.MessageType == WebSocketMessageType.Close)
                    {
                        await socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", token);
                        break;
                    }

                    if (result.MessageType == WebSocketMessageType.Text)
                    {
                        var text = Encoding.UTF8.GetString(buffer, 0, result.Count);

                        // Ook voor tekst via WebSocket: dezelfde route als de textbox.
                        _viewModel.ShowTextFromAnyThread(text);
                    }
                }
            }
            catch
            {
                // socket kapot → negeren
            }
        }

        //QQQ>

        //public readonly struct RawButtonEvent
        //{
        //    public int Strip { get; }
        //    public int ButtonIndex { get; }
        //    public bool IsPress { get; }

        //    public RawButtonEvent(int strip, int buttonIndex, bool isPress)
        //    {
        //        Strip = strip;
        //        ButtonIndex = buttonIndex;
        //        IsPress = isPress;
        //    }
        //}

        public readonly struct RawButtonEvent
        {
            public uint MsgType { get; }
            public int Strip { get; }
            public int ButtonIndex { get; }
            public bool IsPress { get; }

            public RawButtonEvent(uint msgType, int strip, int buttonIndex, bool isPress)
            {
                MsgType = msgType;
                Strip = strip;
                ButtonIndex = buttonIndex;
                IsPress = isPress;
            }
        }


        public void HandleRawKeyFromSam(uint msgType, uint unitId, uint strip, uint param)
        {
            Log($"Raw SAM event: msgType={msgType}, unit={unitId}, strip={strip}, param={param}");

            bool isPress = true; // later you can distinguish press/release if needed

            //>
            // SAM encodes certain keys as bitmasks. Convert them into button indices.
            int buttonIndex = (int)param;

            // If power-of-two → convert to index
            if (param != 0 && (param & (param - 1)) == 0) // check power-of-two
            {
                // Find the bit position (0-based)
                buttonIndex = (int)Math.Log(param, 2);
                Log($"[MAP] Bitmask detected: param={param}, mapped buttonIndex={buttonIndex}");
            }

            //<


            //var raw = new RawButtonEvent(
            //    strip: (int)strip,
            //    buttonIndex: (int)param,
            //    isPress: isPress
            //);

            var raw = new RawButtonEvent(
                msgType: msgType,
                strip: (int)strip,
                buttonIndex: (int)param,
                isPress: isPress
            );

            BrailleLogicalEvent? logical = null;

            if (_keymapMapper != null)
            {
                logical = _keymapMapper.Map(raw);

                if (logical == null)
                {
                    Log($"Raw event not mapped by keymap: strip={raw.Strip}, button={raw.ButtonIndex}");
                }
                else
                {
                    Log($"Mapped event → Kind={logical.Value.Kind}, Name={logical.Value.Name}, CursorIndex={logical.Value.CursorIndex}, IsPress={logical.Value.IsPress}");
                }
            }
            else
            {
                Log("No keymap mapper available, sending only raw data.");
            }

            // Build combined message: RAW + (optional) mapped info
            var msg = new BrailleKeyEventMessage
            {
                // raw
                MsgType = msgType,
                UnitId = unitId,
                Strip = (int)strip,
                ButtonIndex = (int)param,
                IsPress = isPress,

                // mapped (if we have it)
                Kind = logical?.Kind ?? BrailleLogicalEventKind.Unknown,
                CursorIndex = logical?.CursorIndex ?? -1,
                Name = logical?.Name
            };

            _ = BroadcastKeyEventAsync(msg);
        }

        public enum BrailleLogicalEventKind
        {
            Unknown = 0,
            CursorRouting = 1,
            ThumbKey = 2
        }

        public readonly struct BrailleLogicalEvent
        {
            public BrailleLogicalEventKind Kind { get; init; }

            // Voor CursorRouting: index van de cursor routing toets
            public int CursorIndex { get; init; }

            // Voor ThumbKey: logische naam, bv. "LeftThumbKeyPressed"
            public string? Name { get; init; }

            // true = indrukken, false = loslaten (voor later)
            public bool IsPress { get; init; }
        }

        private async Task BroadcastLogicalEventAsync(BrailleLogicalEvent ev, CancellationToken token = default)
        {
            if (_sockets.IsEmpty)
                return;

            var json = JsonSerializer.Serialize(ev);
            Log($"Broadcasting logical event JSON: {json}");

            var bytes = Encoding.UTF8.GetBytes(json);
            var segment = new ArraySegment<byte>(bytes);

            foreach (var socket in _sockets)
            {
                if (socket.State == WebSocketState.Open)
                {
                    try
                    {
                        await socket.SendAsync(
                            segment,
                            WebSocketMessageType.Text,
                            endOfMessage: true,
                            cancellationToken: token);
                    }
                    catch
                    {
                        // eventueel later: socket opruimen
                    }
                }
            }
        }


        //--
        public sealed class JsonKeymapMapper
        {
            private readonly BrailleKeymap _km;

            public JsonKeymapMapper(BrailleKeymap km)
            {
                _km = km;
            }

         

            public BrailleLogicalEvent? Map(RawButtonEvent raw)
            {
                //
                // 1) Cursor routing:
                //    msgType == 11  AND  on cursorRoutingStrip
                //    → CursorRouting event with CursorIndex = raw.ButtonIndex
                //
                if (raw.MsgType == 11 && raw.Strip == _km.CursorRoutingStrip)
                {
                    return new BrailleLogicalEvent
                    {
                        Kind = BrailleLogicalEventKind.CursorRouting,
                        CursorIndex = raw.ButtonIndex,
                        IsPress = raw.IsPress
                    };
                }

                //
                // 2) Thumb keys:
                //    msgType == 8  AND  on thumbKeyStrip  AND  found in thumbKeys dict
                //
                if (raw.MsgType == 8 &&
                    raw.Strip == _km.ThumbKeyStrip &&
                    _km.ThumbKeys.TryGetValue(raw.ButtonIndex, out var name))
                {
                    return new BrailleLogicalEvent
                    {
                        Kind = BrailleLogicalEventKind.ThumbKey,
                        Name = name,
                        IsPress = raw.IsPress
                    };
                }

                //
                // 3) OtherKeys (optional)
                //
                if (_km.OtherKeys != null)
                {
                    foreach (var other in _km.OtherKeys)
                    {
                        if (raw.Strip == other.Strip &&
                            raw.ButtonIndex == other.Button)
                        {
                            return new BrailleLogicalEvent
                            {
                                Kind = BrailleLogicalEventKind.ThumbKey, // Or create another enum kind later
                                Name = other.Event,
                                IsPress = raw.IsPress
                            };
                        }
                    }
                }

                //
                // 4) Not mapped
                //
                return null;
            }



        }

        public sealed class BrailleKeyEventMessage
        {
            // RAW from SAM
            public uint MsgType { get; set; }
            public uint UnitId { get; set; }
            public int Strip { get; set; }
            public int ButtonIndex { get; set; }
            public bool IsPress { get; set; }

            // MAPPED (logical)
            public BrailleLogicalEventKind Kind { get; set; }
            public int CursorIndex { get; set; }
            public string? Name { get; set; }
        }

        private async Task BroadcastKeyEventAsync(BrailleKeyEventMessage msg, CancellationToken token = default)
        {
            if (_sockets.IsEmpty)
                return;

            var json = JsonSerializer.Serialize(msg);
            Log($"Broadcasting key event JSON: {json}");

            var bytes = Encoding.UTF8.GetBytes(json);
            var segment = new ArraySegment<byte>(bytes);

            foreach (var socket in _sockets)
            {
                if (socket.State == WebSocketState.Open)
                {
                    try
                    {
                        await socket.SendAsync(
                            segment,
                            WebSocketMessageType.Text,
                            endOfMessage: true,
                            cancellationToken: token);
                    }
                    catch
                    {
                        // later you can remove broken sockets here
                    }
                }
            }
        }


        public void SetKeymap(BrailleKeymap? keymap)
        {
            _keymapMapper = keymap != null ? new JsonKeymapMapper(keymap) : null;

            if (keymap == null)
            {
                Log("No keymap loaded (SetKeymap(null)). Raw events will be ignored.");
            }
            else
            {
                Log($"Keymap loaded for device '{keymap.DeviceId}' – {keymap.Description}");

                if (!string.IsNullOrEmpty(keymap.SourceFilePath))
                {
                    Log($"Keymap source file: {keymap.SourceFilePath}");
                }
            }
        }




        //QQQ<

        /// <summary>
        /// Verstuur een braille key event als JSON naar alle aangesloten WebSockets.
        /// (Geen SAM-calls hier, dus geen dispatcher nodig.)
        /// </summary>
        public async Task BroadcastBrailleKeyEventAsync(
            uint msgType,
            uint unitId,
            uint strip,
            uint param)
        {
            var payload = new
            {
                type = MapMsgType(msgType),
                msgType,
                unitId,
                strip,
                param,
                timestamp = DateTime.UtcNow
            };

            var json = JsonSerializer.Serialize(payload);
            var bytes = Encoding.UTF8.GetBytes(json);
            var segment = new ArraySegment<byte>(bytes);

            foreach (var socket in _sockets.ToArray())
            {
                if (socket.State == WebSocketState.Open)
                {
                    try
                    {
                        await socket.SendAsync(segment, WebSocketMessageType.Text, true, CancellationToken.None);
                    }
                    catch
                    {
                        // kapotte socket negeren
                    }
                }
            }
        }

        private static string MapMsgType(uint msgType)
        {
            return msgType switch
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
        }

        public void Dispose()
        {
            _cts.Cancel();

            try
            {
                if (_listener.IsListening)
                {
                    _listener.Stop();
                }

                _listener.Close();
            }
            catch
            {
                // negeren
            }

            foreach (var s in _sockets)
            {
                try { s.Dispose(); } catch { }
            }
        }
    }
}
