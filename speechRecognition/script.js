let port, writer;
let isListening = false;
let isSigning = false;

const COMMANDS = ['open', 'spread', 'fist', 'point', 'thumbs up'];


document.getElementById('connect').onclick = async () => {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        writer = port.writable.getWriter();
        document.getElementById('status').innerText = "Status: Connected!";
    } catch (err) {
        alert("err");
    }
};


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
        const encoder = new TextEncoder();
        if (COMMANDS.includes(transcript)) {
            await writer.write(encoder.encode(transcript + "\n"));
            console.log(`Signing command: "${transcript}"`);
        } else {
            for (const letter of transcript.toUpperCase()) {
                if (/[A-Z]/.test(letter)) {
                    await writer.write(encoder.encode(letter + "\n"));
                    console.log(`Signing letter: "${letter}"`);
                }
            }
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