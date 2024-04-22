<script>
    let playerName = '';
    let showContainer = true;
    let characterDialogue = ''; // Add this line
    let dialogueIndex = 0; // Add this line
    let intervalId = null; // Add this line

    function startGame() {
        if (playerName != null && playerName != '') {
            const playerData = { name: playerName };
            localStorage.setItem('playerData', JSON.stringify(playerData));
            showContainer = false;
            startDialogue(); // Add this line
        }
    }

    function startDialogue() {
        const dialogues = 'First line of dialogue\nSecond line of dialogue';
        intervalId = setInterval(() => {
            if (dialogueIndex < dialogues.length) {
                characterDialogue += dialogues[dialogueIndex];
                dialogueIndex++;
            } else {
                clearInterval(intervalId);
            }
        }, 100); // Change this value to adjust the speed of the text display
    }

    function showAllText() { // Add this function
        if (intervalId) {
            clearInterval(intervalId);
            characterDialogue = 'Character Dialogue';
        }
    }
    // onMount(() => {
    //     const dialogueBoxes = document.getElementsByClassName('dialogue-box');
    //     for (let i = 0; i < dialogueBoxes.length; i++) {
    //         dialogueBoxes[i].addEventListener('click', showAllText);
    //         dialogueBoxes[i].addEventListener('keydown', (event) => {
    //             if (event.keyCode === 32) showAllText();
    //         });
    //         dialogueBoxes[i].setAttribute('tabindex', '0');
    //     }
    // });
</script>

<head>
    <link rel="stylesheet" href="src/routes/start.css">
</head>

{#if showContainer}
<div class="container">
    <input bind:value={playerName} placeholder="Please enter player name" />
    <button on:click={startGame}>Start</button>
</div>
{/if}

{#if !showContainer}
<div class="dialogue-box">
    <div class="character-info"> <!-- Add this line -->
        <div class="character-avatar">
            <img src="head/avatar.jpeg" alt="Character Avatar">
        </div>
        <div class="character-name">{ playerName }</div>
    </div> <!-- Add this line -->
    <div class="character-dialogue">{ characterDialogue }</div>
</div>
{/if}
