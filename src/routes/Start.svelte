<script>
    let playerName = '';
    let showContainer = true;
    let characterDialogue = ''; // Add this line
    let dialogueIndex = 0; // Add this line
    let intervalId = null; // Add this line
    let backpack = {}; // 新增這行
    let mana = 10; // 新增這行

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
//------------------- HANDLE DATA -------------------

// import { onMount } from 'svelte';

// let gameData = null;

// onMount(async () => {
//     const response = await fetch('output.json');
//     gameData = await response.json();
// });
//------------------- HANDLE DATA -------------------

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
<div class="character-mana">Mana: { mana }</div> <!-- 新增這行 -->
<div class="character-backpack">Backpack: { backpack }</div> <!-- 新增這行 -->
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

<!-- {#if gameData}
<div class="game-data">
    <h2>{gameData.name} by {gameData.author}</h2>
    <h3>Player</h3>
    <p>Starter: {gameData.player.starter}</p>
    <p>Inventory: {gameData.player.inventory.join(', ')}</p>

    <h3>Scenes</h3>
    {#each Object.entries(gameData.scene) as [key, value]}
    <div>
        <strong>{value.name}:</strong>
        <img src={value.background} alt={value.name} />
    </div>
    {/each}

    <h3>Characters</h3>
    {#each Object.entries(gameData.character) as [key, value]}
    <div>
        <strong>{value.name}:</strong>
        <img src={value.avatar} alt={value.name} />
    </div>
    {/each}

    <h3>Items</h3>
    {#each Object.entries(gameData.item) as [key, value]}
    <div>
        <strong>{value.name}:</strong>
        <img src={value.icon} alt={value.name} />
        <p>{value.description}</p>
    </div>
    {/each}

    <h3>Events</h3>
    {#each Object.entries(gameData.event) as [key, value]}
    <div>
        <strong>{key}:</strong>
        <p>Scene: {value.scene}</p>
        <p>Dialogue: {gameData.dialogue[value.dialogue].text}</p>
    </div>
    {/each}
</div>
{/if} -->