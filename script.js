let port, writer;
let isListening = false;


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

    if (writer && (transcript === "on" || transcript === "off")) {
        const encoder = new TextEncoder();
        await writer.write(encoder.encode(transcript + "\n"));
        console.log("Sent command:", transcript);
    }
};


recognition.onend = () => {
    if (isListening) {
        recognition.start();
    }
};


document.getElementById('start').onclick = () => {
    isListening = true;
    recognition.start();
};

document.getElementById('stop').onclick = () => {
    isListening = false;
    recognition.stop();
};