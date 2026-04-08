let port, writer, reader;
let isListening = false;
let isSigning = false;
let resolveCurrentSign = null;

const FALLBACK_COMMANDS = ['open', 'spread', 'fist', 'point', 'thumbs up', 'rock horns'];
let COMMANDS = [...FALLBACK_COMMANDS];
const encoder = new TextEncoder();


document.getElementById('connect').onclick = async () => {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        writer = port.writable.getWriter();
        reader = port.readable.getReader();
        startReader();
        // Explicitly request commands in case startup serial lines were missed.
        await writer.write(encoder.encode("GET_COMMANDS\n"));
        document.getElementById('status').innerText = "Status: Connected!";
    } catch (err) {
        alert("err");
    }
};


async function startReader() {
    const decoder = new TextDecoder();
    let buffer = '';
    try {
        while (true) {
            const { value, done } = await reader.read();
            if (done) break;
            buffer += decoder.decode(value);
            const lines = buffer.split('\n');
            buffer = lines.pop();
            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed) continue;
                console.log('Serial:', trimmed);
                
                // Parse command list from Arduino
                if (trimmed.startsWith('COMMANDS_LIST:[')) {
                    const jsonStr = trimmed.substring('COMMANDS_LIST:'.length);
                    try {
                        const parsed = JSON.parse(jsonStr);
                        if (Array.isArray(parsed) && parsed.length > 0) {
                            COMMANDS = parsed
                                .map((c) => String(c).toLowerCase().trim())
                                .filter(Boolean);
                        }
                        console.log('Loaded commands from Arduino:', COMMANDS);
                        document.getElementById('status').innerText = `Commands loaded: ${COMMANDS.join(', ')}`;
                    } catch (e) {
                        console.error('Failed to parse commands list:', e);
                        document.getElementById('status').innerText = 'Using fallback commands';
                    }
                }
                
                if (trimmed === 'DONE' && resolveCurrentSign) {
                    resolveCurrentSign();
                    resolveCurrentSign = null;
                }
            }
        }
    } catch (e) {
        console.error('Reader error:', e);
    }
}


async function sendAndWait(command) {
    return new Promise((resolve) => {
        resolveCurrentSign = resolve;
        writer.write(encoder.encode(command + "\n"));
    });
}


function parseTokens(text) {
    const tokens = [];
    const normalizedCommands = COMMANDS.map((c) => c.toLowerCase());
    const words = text.split(/\s+/).filter(Boolean);
    let i = 0;
    while (i < words.length) {
        let matched = false;
        for (let len = words.length - i; len > 1; len--) {
            const phrase = words.slice(i, i + len).join(' ');
            if (normalizedCommands.includes(phrase)) {
                tokens.push(phrase);
                i += len;
                matched = true;
                break;
            }
        }
        if (!matched) {
            if (normalizedCommands.includes(words[i])) {
                tokens.push(words[i]);
            } else {
                for (const ch of words[i].toUpperCase()) {
                    if (/[A-Z]/.test(ch)) tokens.push(ch);
                }
            }
            i++;
        }
    }
    return tokens;
}


const recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
recognition.continuous = false;
recognition.lang = 'en-US';


recognition.onresult = async (event) => {
    const transcript = event.results[event.results.length - 1][0].transcript.toLowerCase().trim();
    document.getElementById('status').innerText = "Last heard: " + transcript;

    isListening = false;
    document.getElementById('toggle').innerText = "Start Listening";

    if (writer) {
        isSigning = true;
        document.getElementById('toggle').disabled = true;
        const tokens = parseTokens(transcript);
        console.log('Tokens:', tokens);
        for (const token of tokens) {
            console.log(`Signing: "${token}"`);
            await sendAndWait(token);
        }
        isSigning = false;
        document.getElementById('toggle').disabled = false;
    }
};


recognition.onend = () => {
    isListening = false;
    document.getElementById('toggle').innerText = "Start Listening";
};


document.getElementById('toggle').onclick = () => {
    if (isSigning) return;
    isListening = !isListening;
    if (isListening) {
        recognition.start();
        document.getElementById('toggle').innerText = "Stop Listening";
    } else {
        recognition.stop();
        document.getElementById('toggle').innerText = "Start Listening";
    }
};
