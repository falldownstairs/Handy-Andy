const sceneStretch = document.getElementById('scene-stretch');

// -- Helpers ------------------------------------------------
function p(x, y) { return `${x},${y}`; }
function tri(ax, ay, bx, by, cx, cy) { return `M ${p(ax, ay)} L ${p(bx, by)} L ${p(cx, cy)} Z`; }
function makePath(d, fill) {
  const el = document.createElementNS('http://www.w3.org/2000/svg', 'path');
  el.setAttribute('d', d);
  el.setAttribute('fill', fill);
  return el;
}

const SCENE_COLORS = {
  blushBeige: 'var(--color-2)',
  warmSand: 'var(--color-3)',
  mutedTan: 'var(--color-4)',
  mediumTan: 'var(--color-6)',
  goldenTan: 'var(--color-7)'
};

const BASE_W = 1024;
const BASE_H = 576;

// -- Mountain Data ------------------------------------------
const farMtns = [[0.1, 0.33, -0.1, 0.48], [0.25, 0.27, 0, 0.62], [0.5, 0.08, 0.22, 0.7], [0.61, 0.15, 0.32, 0.86], [0.75, 0.27, 0.58, 1.12], [0.9, 0.25, 0.82, 1.2]];
const nearMtns = [[0.05, 0.57, -0.18, 0.3], [0.23, 0.39, -0.02, 0.54], [0.44, 0.43, 0.2, 0.65], [0.58, 0.33, 0.34, 0.82], [0.76, 0.47, 0.54, 0.98], [0.91, 0.55, 0.72, 1.1], [1.06, 0.45, 0.9, 1.3]];
const midMtns = [
  [0.08, 0.62, -0.12, 0.38], [0.20, 0.55, -0.06, 0.62], [0.38, 0.54, -0.14, 0.67],
  [0.55, 0.52, 0.2, 0.9], [0.70, 0.6, 0.50, 1.3], [0.85, 0.58, 0.68, 1.6], [1.00, 0.62, 0.82, 1.18],
];
const frontMtns = [
  [0.05, 0.75, -0.25, 0.40], [0.18, 0.70, -0.12, 0.56], [0.35, 0.73, 0.1, 0.65],
  [0.50, 0.67, 0.22, 0.87], [0.65, 0.72, 0.29, 0.99], [0.80, 0.71, 0.52, 1.20], [0.95, 0.74, 0.68, 1.35],
];

function renderMountains(groupId, mtns, color) {
  const g = document.getElementById(groupId);
  mtns.forEach(([ax, ay, bl, br]) => {
    g.appendChild(makePath(tri(BASE_W * ax, BASE_H * ay, BASE_W * bl, BASE_H, BASE_W * br, BASE_H), color));
  });
}

function getMountainEdgeY(mtns, nx) {
  let minY = 1.0;
  for (const [ax, ay, bl, br] of mtns) {
    if (nx < bl || nx > br) continue;
    const y = nx <= ax ? 1.0 + (ay - 1.0) * (nx - bl) / (ax - bl) : ay + (1.0 - ay) * (nx - ax) / (br - ax);
    if (y < minY) minY = y;
  }
  return minY;
}

function drawTree(group, x, y, scale, color) {
  const trunkW = 10 * scale;
  const trunkH = 15 * scale;
  const trunk = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
  trunk.setAttribute('x', x - trunkW / 2);
  trunk.setAttribute('y', y - trunkH);
  trunk.setAttribute('width', trunkW);
  trunk.setAttribute('height', trunkH);
  trunk.setAttribute('fill', color);
  group.appendChild(trunk);

  for (let i = 0; i < 3; i++) {
    const tierW = (40 - (i * 10)) * scale;
    const tierH = 25 * scale;
    const bottomY = y - trunkH - (i * 15 * scale);
    group.appendChild(makePath(tri(x, bottomY - tierH, x - tierW / 2, bottomY, x + tierW / 2, bottomY), color));
  }
}

function renderTreesOnMountains(groupId, mtns, color, count, baseScale, stemOffset) {
  const group = document.getElementById(groupId);
  for (let i = 0; i < count; i++) {
    const nx = (i + 0.5) / count;
    const edgeY = getMountainEdgeY(mtns, nx);
    if (edgeY >= 0.98) continue;
    const scale = baseScale * (0.8 + 0.4 * (((i * 7 + 3) % 5) / 4));
    drawTree(group, BASE_W * nx, BASE_H * (edgeY + stemOffset), scale, color);
  }
}

// -- Ground Animation Logic (Solid Geometry) -----------------
let leftGroundPath, rightGroundPath;
let animProgress = 1;

function initGround(W, H) {
  sceneStretch.setAttribute('viewBox', `0 0 ${W} ${H}`);
  const ground = document.getElementById('ground');
  ground.innerHTML = '';

  // Initialize the empty paths
  leftGroundPath = makePath('', SCENE_COLORS.blushBeige);
  rightGroundPath = makePath('', SCENE_COLORS.blushBeige);

  ground.appendChild(leftGroundPath);
  ground.appendChild(rightGroundPath);

  // Render the current state instantly
  updateGroundPaths(W, H, animProgress);
}

function updateGroundPaths(W, H, progress) {
  if (!leftGroundPath || !rightGroundPath) return;

  const px = W / 2;
  const py = H;

  // Smooth easing (Cubic Ease In-Out)
  const ease = progress < 0.5 ? 4 * progress * progress * progress : 1 - Math.pow(-2 * progress + 2, 3) / 2;

  // If at the very beginning, draw full screen rectangles with a tiny 1px overlap
  // in the center so there are no visual hairline cracks before it opens
  if (ease < 0.001) {
    leftGroundPath.setAttribute('d', `M ${px + 1},${py} L 0,${H} L 0,0 L ${px + 1},0 Z`);
    rightGroundPath.setAttribute('d', `M ${px - 1},${py} L ${W},${H} L ${W},0 L ${px - 1},0 Z`);
    return;
  }

  // Calculate the angles. It starts pointing straight up (-PI/2) and sweeps outward.
  const startAngle = -Math.PI / 2;

  // Calculate the final resting angles of your specific valley triangles
  const targetAngleLeft = Math.atan2((H * 0.6) - py, 0 - px);
  const targetAngleRight = Math.atan2((H * 0.55) - py, W - px);

  const angleLeft = startAngle + ease * (targetAngleLeft - startAngle);
  const angleRight = startAngle + ease * (targetAngleRight - startAngle);

  // --- Left Curtain Math ---
  let dLeft = '';
  // Find where the sweeping line intersects the top edge of the screen
  const x_top = px - py * (Math.cos(angleLeft) / Math.sin(angleLeft));

  if (x_top >= 0) {
    // If the line hits the top edge, the shape covers the bottom left corner up to the top
    dLeft = `M ${px},${py} L 0,${H} L 0,0 L ${x_top},0 Z`;
  } else {
    // If the line swings past the top left corner, it now hits the left wall
    const y_left = py - px * (Math.sin(angleLeft) / Math.cos(angleLeft));
    dLeft = `M ${px},${py} L 0,${H} L 0,${y_left} Z`;
  }

  // --- Right Curtain Math ---
  let dRight = '';
  const x_top_r = px - py * (Math.cos(angleRight) / Math.sin(angleRight));

  if (x_top_r <= W) {
    // Hits top edge
    dRight = `M ${px},${py} L ${W},${H} L ${W},0 L ${x_top_r},0 Z`;
  } else {
    // Hits right edge
    const y_right = py + (W - px) * (Math.sin(angleRight) / Math.cos(angleRight));
    dRight = `M ${px},${py} L ${W},${H} L ${W},${y_right} Z`;
  }

  // Update the SVG attributes directly
  leftGroundPath.setAttribute('d', dLeft);
  rightGroundPath.setAttribute('d', dRight);
}

function playAnimation() {
  const duration = 1000;
  const startTime = performance.now();

  function step(time) {
    const elapsed = time - startTime;
    animProgress = Math.min(elapsed / duration, 1);

    updateGroundPaths(window.innerWidth, window.innerHeight, animProgress);

    if (animProgress < 1) {
      requestAnimationFrame(step);
    }
  }
  requestAnimationFrame(step);
}

// -- Init ----------------------------------------------------
renderMountains('mountains-far', farMtns, SCENE_COLORS.warmSand);
renderMountains('mountains-near', nearMtns, SCENE_COLORS.mutedTan);
renderMountains('mountains-mid', midMtns, SCENE_COLORS.mediumTan);
renderTreesOnMountains('trees-mid', midMtns, SCENE_COLORS.mediumTan, 18, 0.55, 0.01);
renderMountains('mountains-front', frontMtns, SCENE_COLORS.goldenTan);
renderTreesOnMountains('trees-front', frontMtns, SCENE_COLORS.goldenTan, 20, 0.65, 0.005);

// Set up the scene
initGround(window.innerWidth, window.innerHeight);

// Keep the geometric math responsive to window resizing
window.addEventListener('resize', () => {
  initGround(window.innerWidth, window.innerHeight);
});

// Face follows cursor
const characterFace = document.getElementById('character-face');
document.addEventListener('mousemove', (e) => {
  if (!characterFace) return;

  const xPercent = e.clientX / window.innerWidth;
  const yPercent = e.clientY / window.innerHeight;

  // Shift face smoothly by up to max 12.5px in any direction
  const dx = (xPercent - 0.5) * 25;
  const dy = (yPercent - 0.5) * 25;

  characterFace.setAttribute('transform', `translate(${dx}, ${dy})`);
});

// -- UI Integration Logic -----------------------------------
let port, writer, reader;
let isListening = false;
let currentMode = 'voice';
let isSigning = false;
let resolveCurrentSign = null;

const FALLBACK_COMMANDS = ['open', 'spread', 'fist', 'point', 'thumbs up', 'rock horns'];
let COMMANDS = [...FALLBACK_COMMANDS];
const encoder = new TextEncoder();
const SERVO_COUNT = 9;
const servoPercents = new Array(SERVO_COUNT).fill(0);
const servoUiDisplayed = new Array(SERVO_COUNT).fill(0);
const servoUiTarget = new Array(SERVO_COUNT).fill(0);
let rawSendTimer = null;
let servoUiAnimFrame = null;
const activeDrag = new Array(SERVO_COUNT).fill(false);

const SERVO_LABELS = [
  'Index',
  'Middle',
  'Ring',
  'Pinkie',
  'Splay IM',
  'Splay MR',
  'Splay RP',
  'Thumb Bend',
  'Thumb Cross'
];

function renderServoRows() {
  const container = document.getElementById('servo-rows');
  container.innerHTML = SERVO_LABELS.map((label, i) => `
    <div class="servo-row">
      <label for="servo-${i}">(${i}) ${label}</label>
      <input class="servo-slider" id="servo-${i}" type="range" min="0" max="100" step="0.1" value="0" data-servo-index="${i}" disabled />
      <span class="servo-value" id="servo-value-${i}">0%</span>
    </div>
  `).join('');
}

renderServoRows();

const textInput = document.getElementById('text-input');
const toggleListenBtn = document.getElementById('toggle-listen');
const servoSliders = document.querySelectorAll('.servo-slider');
const logsEl = document.getElementById('logs');

function setInputLocked(locked) {
  textInput.disabled = locked;
  toggleListenBtn.disabled = locked;
  setServoControlsDisabled(locked || !writer);
}

function setServoControlsDisabled(disabled) {
  for (const slider of servoSliders) {
    slider.disabled = disabled;
  }
}

function updateSliderFill(slider) {
  const pct = ((slider.value - slider.min) / (slider.max - slider.min)) * 100;
  slider.style.setProperty('--val', pct + '%');
}

function renderServoValue(index, value) {
  const clamped = Math.max(0, Math.min(100, value));
  const slider = document.getElementById(`servo-${index}`);
  const valueLabel = document.getElementById(`servo-value-${index}`);
  if (slider && !activeDrag[index]) {
    slider.value = clamped.toFixed(1);
    updateSliderFill(slider);
  }
  if (valueLabel) valueLabel.innerText = `${clamped.toFixed(1)}%`;
}

function animateServoUi() {
  let hasPending = false;
  for (let i = 0; i < SERVO_COUNT; i++) {
    const delta = servoUiTarget[i] - servoUiDisplayed[i];
    if (Math.abs(delta) < 0.25) {
      servoUiDisplayed[i] = servoUiTarget[i];
    } else {
      servoUiDisplayed[i] += delta * 0.28;
      hasPending = true;
    }
    renderServoValue(i, servoUiDisplayed[i]);
  }

  if (hasPending) {
    servoUiAnimFrame = requestAnimationFrame(animateServoUi);
  } else {
    servoUiAnimFrame = null;
  }
}

function applyServoState(values) {
  for (let i = 0; i < Math.min(values.length, SERVO_COUNT); i++) {
    if (activeDrag[i]) {
      continue;
    }
    const n = Math.max(0, Math.min(100, Number(values[i]) || 0));
    servoPercents[i] = n;
    servoUiTarget[i] = n;
  }

  if (!servoUiAnimFrame) {
    servoUiAnimFrame = requestAnimationFrame(animateServoUi);
  }
}

function sendRawServoState() {
  if (!writer || isSigning) return;
  const payload = 'raw,' + servoPercents.map((v) => Math.max(0, Math.min(100, Number(v) || 0)).toFixed(1)).join(',');
  writer.write(encoder.encode(payload + '\n'));
}

function scheduleRawServoSend() {
  if (rawSendTimer || !writer || isSigning) return;
  rawSendTimer = setTimeout(() => {
    rawSendTimer = null;
    sendRawServoState();
  }, 50);
}

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

        if (trimmed.startsWith('COMMANDS_LIST:[')) {
          const jsonStr = trimmed.substring('COMMANDS_LIST:'.length);
          try {
            const parsed = JSON.parse(jsonStr);
            if (Array.isArray(parsed) && parsed.length > 0) {
              COMMANDS = parsed
                .map((c) => String(c).toLowerCase().trim())
                .filter(Boolean);
              logMessage(`Commands synced: ${COMMANDS.join(', ')}`, 'sys');
            }
          } catch (e) {
            logMessage('Failed to parse command list; using fallback commands', 'err');
          }
        } else if (trimmed === 'DONE' && resolveCurrentSign) {
          resolveCurrentSign();
          resolveCurrentSign = null;
        } else if (trimmed.startsWith('STATE,')) {
          const values = trimmed.split(',').slice(1);
          if (values.length === SERVO_COUNT) {
            applyServoState(values);
          }
        } else if (trimmed.startsWith('Unknown command:')) {
          logMessage(trimmed, 'err');
        }
      }
    }
  } catch (e) {
    logMessage('Reader error: ' + e.message, 'err');
  }
}

async function sendAndWait(command) {
  return new Promise((resolve) => {
    resolveCurrentSign = resolve;
    writer.write(encoder.encode(command + '\n'));
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

async function signText(text) {
  if (!writer) {
    logMessage('Not connected to Arduino', 'err');
    return;
  }
  isSigning = true;
  setInputLocked(true);
  const tokens = parseTokens(text);
  for (const token of tokens) {
    logMessage(`Signing: "${token}"`, 'sys');
    await sendAndWait(token);
  }
  isSigning = false;
  setInputLocked(false);
}

function logMessage(msg, type = 'sys') {
  const el = document.createElement('div');
  el.className = `log-msg log-${type}`;
  el.innerText = `[${new Date().toLocaleTimeString()}] ${msg}`;
  logsEl.appendChild(el);
  logsEl.scrollTop = logsEl.scrollHeight;
}

logMessage('System Initialized. Waiting for connection...', 'sys');

// Port Connection
document.getElementById('connect-btn').onclick = async () => {
  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });
    writer = port.writable.getWriter();
    reader = port.readable.getReader();
    startReader();
    await writer.write(encoder.encode('GET_COMMANDS\n'));
    setServoControlsDisabled(false);
    logMessage('Arduino Connected Successfully', 'sys');
    document.getElementById('connect-btn').innerText = 'Connected';
    document.getElementById('connect-btn').classList.add('active');
  } catch (err) {
    logMessage('Connection Error: ' + err.message, 'err');
  }
};

// Toggle Modes
const btnVoice = document.getElementById('mode-voice');
const btnText = document.getElementById('mode-text');
const containerVoice = document.getElementById('voice-container');
const containerText = document.getElementById('text-container');

function setMode(mode) {
  currentMode = mode;
  if (mode === 'voice') {
    btnVoice.classList.add('active');
    btnText.classList.remove('active');
    containerVoice.classList.add('active-container');
    containerText.classList.remove('active-container');
  } else {
    btnText.classList.add('active');
    btnVoice.classList.remove('active');
    containerText.classList.add('active-container');
    containerVoice.classList.remove('active-container');
  }
}

btnVoice.onclick = () => setMode('voice');
btnText.onclick = () => setMode('text');

servoSliders.forEach((slider) => {
  const i = Number(slider.dataset.servoIndex);
  updateSliderFill(slider);

  const beginDrag = () => {
    activeDrag[i] = true;
  };

  const endDrag = () => {
    activeDrag[i] = false;
  };

  slider.addEventListener('pointerdown', beginDrag);
  slider.addEventListener('pointerup', endDrag);
  slider.addEventListener('pointercancel', endDrag);
  slider.addEventListener('blur', endDrag);

  slider.addEventListener('input', () => {
    const value = Number(slider.value);
    servoPercents[i] = value;
    servoUiDisplayed[i] = value;
    servoUiTarget[i] = value;
    renderServoValue(i, value);
    updateSliderFill(slider);
    scheduleRawServoSend();
  });
});

textInput.addEventListener('keypress', async (e) => {
  if (e.key === 'Enter' && !isSigning) {
    const val = textInput.value.trim().toLowerCase();
    if (val) {
      logMessage(`Text typed: "${val}"`, 'data');
      textInput.value = '';
      await signText(val);
    }
  }
});

// Voice recognition
const recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
recognition.continuous = false;
recognition.lang = 'en-US';

recognition.onresult = async (event) => {
  const transcript = event.results[event.results.length - 1][0].transcript.toLowerCase().trim();
  logMessage(`Heard: "${transcript}"`, 'data');
  isListening = false;
  updateVoiceToggle();
  await signText(transcript);
};

recognition.onend = () => {
  isListening = false;
  updateVoiceToggle();
};

function updateVoiceToggle() {
  if (isListening) {
    toggleListenBtn.innerText = 'Stop Listening';
    toggleListenBtn.classList.add('active');
  } else {
    toggleListenBtn.innerText = 'Start Listening';
    toggleListenBtn.classList.remove('active');
  }
}

toggleListenBtn.onclick = () => {
  if (isSigning) return;
  isListening = !isListening;
  if (isListening) {
    try {
      recognition.start();
      logMessage('Started listening...', 'sys');
    } catch (e) {
      logMessage('Speech Recognition Error: ' + e.message, 'err');
      isListening = false;
    }
  } else {
    recognition.stop();
    logMessage('Stopped listening.', 'sys');
  }
  updateVoiceToggle();
};

// -- Collapsible Panels -------------------------------------
function setupCollapsible(toggleId, bodyId, chevronId) {
  const toggle = document.getElementById(toggleId);
  const body = document.getElementById(bodyId);
  const chevron = document.getElementById(chevronId);
  let expanded = true;
  toggle.addEventListener('click', () => {
    expanded = !expanded;
    body.classList.toggle('collapsed', !expanded);
    chevron.classList.toggle('collapsed', !expanded);
    toggle.setAttribute('aria-expanded', expanded);
  });
}

setupCollapsible('logs-toggle', 'logs-body', 'logs-chevron');
setupCollapsible('servo-toggle', 'servo-body', 'servo-chevron');

// -- Mouth morphs to circle on left-click hold --------------
const mouthEl = document.getElementById('character-mouth');
const MOUTH_SMILE = 'M -10.4,-170 Q 0,-162 10.4,-170';
const MOUTH_CIRCLE = 'M 0,-175 m -8,0 a 8,8 0 1,0 16,0 a 8,8 0 1,0 -16,0';
let mouthRestoreTimer = null;

document.addEventListener('mousedown', (e) => {
  if (e.button === 0 && mouthEl) {
    clearTimeout(mouthRestoreTimer);
    mouthEl.setAttribute('d', MOUTH_CIRCLE);
    mouthEl.setAttribute('fill', 'var(--color-12)');
    mouthEl.setAttribute('stroke', 'none');
  }
});

document.addEventListener('mouseup', (e) => {
  if (e.button === 0 && mouthEl) {
    mouthRestoreTimer = setTimeout(() => {
      mouthEl.setAttribute('d', MOUTH_SMILE);
      mouthEl.setAttribute('fill', 'none');
      mouthEl.setAttribute('stroke', 'var(--color-12)');
      mouthEl.setAttribute('stroke-width', '3');
      mouthEl.setAttribute('stroke-linecap', 'round');
    }, 400);
  }
});
