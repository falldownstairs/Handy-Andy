let port, writer, reader;
let isListening = false;
let isSigning = false;
let resolveCurrentSign = null;

const COMMANDS = ['open', 'spread', 'fist', 'point', 'thumbs up'];
const encoder = new TextEncoder();


document.getElementById('connect').onclick = async () => {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        writer = port.writable.getWriter();
        reader = port.readable.getReader();
        startReader();
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
    const words = text.split(/\s+/).filter(Boolean);
    let i = 0;
    while (i < words.length) {
        let matched = false;
        for (let len = words.length - i; len > 1; len--) {
            const phrase = words.slice(i, i + len).join(' ');
            if (COMMANDS.includes(phrase)) {
                tokens.push(phrase);
                i += len;
                matched = true;
                break;
            }
        }
        if (!matched) {
            if (COMMANDS.includes(words[i])) {
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
