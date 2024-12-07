function buttonChatbotClose() {
  var x = document.getElementById("chat-container");
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }
}
function buttonMusicClose() {
  var x = document.getElementById("playerr");
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }
}

function closePopup() {
  document.getElementById("popup").style.display = "none";
}
// Toggle the dropdown
function toggleDropdown() {
  const container = document.querySelector('.sensor-container');
  container.classList.toggle('active');
}
// Sidebar functions
function openNav() {
  document.getElementById("mySidebar").style.width = "250px";
  document.getElementById("main").style.marginRight = "250px";
}

function closeNav() {
  document.getElementById("mySidebar").style.width = "0";
  document.getElementById("main").style.marginRight = "0";
}
async function sendMessage() {
  const userInput = document.getElementById('user-input').value;
  if (userInput.trim() === '') return;
  // Display user message
  displayMessage(userInput, 'user');

  // Clear input field
  document.getElementById('user-input').value = '';

  // Get response from chatbot
  const botResponse = await getChatbotResponse(userInput);

  // Display bot response
  displayMessage(botResponse, 'bot');
}

// Function to display messages in the chat box
function displayMessage(message, sender) {
  const messageDiv = document.createElement('div');
  messageDiv.classList.add('message', sender + '-message');
  messageDiv.textContent = message;
  document.getElementById('chat-box').appendChild(messageDiv);

  // Scroll to the bottom
  const chatBox = document.getElementById('chat-box');
  chatBox.scrollTop = chatBox.scrollHeight;
}

// Function to get the chatbot's response from the OpenAI API
async function getChatbotResponse(userInput) {
  const apiKey = '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';  // Replace with your actual OpenAI API key

  const response = await fetch('https://api.openai.com/v1/chat/completions', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${apiKey}`,
    },
    body: JSON.stringify({
      model: 'gpt-3.5-turbo', // Use the appropriate model
      messages: [
        { role: 'user', content: userInput },
      ],
    }),
  });

  const data = await response.json();

  if (data.choices && data.choices.length > 0) {
    return data.choices[0].message.content; // Return the bot's response
  } else {
    return 'Nigga, stfu. Quit yappin.';
  }
};

//Music player
let now_playing = document.querySelector(".now-playing");
let track_art = document.querySelector(".track-art");
let track_name = document.querySelector(".track-name");
let track_artist = document.querySelector(".track-artist");

let playpause_btn = document.querySelector(".playpause-track");
let next_btn = document.querySelector(".next-track");
let prev_btn = document.querySelector(".prev-track");

let seek_slider = document.querySelector(".seek_slider");
let volume_slider = document.querySelector(".volume_slider");
let curr_time = document.querySelector(".current-time");
let total_duration = document.querySelector(".total-duration");

let track_index = 0;
let isPlaying = false;
let updateTimer;

// Create new audio element
let curr_track = document.createElement('audio');

// Define the tracks that have to be played
let track_list = [
  {
    name: "Happy song",
    artist: "Played when sad",
    image: "https://i.pinimg.com/originals/4c/ea/03/4cea031fa751e7658f5e0355def16f29.gif",
    path: "./assets/song/happy.mp3"
  },
  {
    name: "Energetic song",
    artist: "Played when happy",
    image: "https://media.tenor.com/jvyizE8wJokAAAAM/pikachu-excited.gif",
    path: "./assets/song/energetic.mp3"
  },
  {
    name: "Phonk",
    artist: "Played when very happy",
    image: "https://media.tenor.com/R29fNQ7Vaf8AAAAM/phonk-skull.gif",
    path: "./assets/song/phonk.mp3",
  },
];
 
function loadTrack(track_index) {
  clearInterval(updateTimer);
  resetValues();
  curr_track.src = track_list[track_index].path;
  curr_track.load();

  track_art.style.backgroundImage = "url(" + track_list[track_index].image + ")";
  track_name.textContent = track_list[track_index].name;
  track_artist.textContent = track_list[track_index].artist;
  now_playing.textContent = "PLAYING " + (track_index + 1) + " OF " + track_list.length;

  updateTimer = setInterval(seekUpdate, 1000);
  curr_track.addEventListener("ended", nextTrack);
}

function resetValues() {
  curr_time.textContent = "00:00";
  total_duration.textContent = "00:00";
  seek_slider.value = 0;
}

// Load the first track in the tracklist
loadTrack(track_index);

function playpauseTrack() {
  if (!isPlaying) playTrack();
  else pauseTrack();
}

function playTrack() {
  curr_track.play();
  isPlaying = true;
  playpause_btn.innerHTML = '<i class="fa fa-pause-circle fa-5x"></i>';
}

function pauseTrack() {
  curr_track.pause();
  isPlaying = false;
  playpause_btn.innerHTML = '<i class="fa fa-play-circle fa-5x"></i>';;
}

function nextTrack() {
  if (track_index < track_list.length - 1)
    track_index += 1;
  else track_index = 0;
  loadTrack(track_index);
  playTrack();
}

function prevTrack() {
  if (track_index > 0)
    track_index -= 1;
  else track_index = track_list.length;
  loadTrack(track_index);
  playTrack();
}

function seekTo() {
  let seekto = curr_track.duration * (seek_slider.value / 100);
  curr_track.currentTime = seekto;
}

function setVolume() {
  curr_track.volume = volume_slider.value / 100;
}

function seekUpdate() {
  let seekPosition = 0;

  if (!isNaN(curr_track.duration)) {
    seekPosition = curr_track.currentTime * (100 / curr_track.duration);

    seek_slider.value = seekPosition;

    let currentMinutes = Math.floor(curr_track.currentTime / 60);
    let currentSeconds = Math.floor(curr_track.currentTime - currentMinutes * 60);
    let durationMinutes = Math.floor(curr_track.duration / 60);
    let durationSeconds = Math.floor(curr_track.duration - durationMinutes * 60);

    if (currentSeconds < 10) { currentSeconds = "0" + currentSeconds; }
    if (durationSeconds < 10) { durationSeconds = "0" + durationSeconds; }
    if (currentMinutes < 10) { currentMinutes = "0" + currentMinutes; }
    if (durationMinutes < 10) { durationMinutes = "0" + durationMinutes; }

    curr_time.textContent = currentMinutes + ":" + currentSeconds;
    total_duration.textContent = durationMinutes + ":" + durationSeconds;
  }
}
