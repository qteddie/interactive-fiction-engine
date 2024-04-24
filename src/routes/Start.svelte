<script>
    import { pushState, replaceState } from '$app/navigation';

    // Use pushState to add a new entry to the history stack
    // pushState('/', { replace: true, keepfocus: true });
    //------------------- HANDLE DATA -------------------
    import { onMount, onDestroy } from 'svelte';
    
    let divElement;

    function handleKeydown(event) {
        if (event.key === ' ') {
            nextDialogue();
        }
    }
    function handleClick() {
        // window.alert('Hello');
        nextDialogue();
    }

    onDestroy(() => {
        if (divElement) {
            divElement.removeEventListener('keydown', handleKeydown);
        }
    });
    let gameData = null;
    let currentScene = null;
    let currentCharacter = null;
    let currentItem = null;
    let dialogueKeys = null;
    let intervalId; // Add this line
    let currentDialogueIndex = 0;
    let currentDialogue = '';
    let playerName = '';
    let showContainer = true;
    let backpack = {}; // 新增這行
    let mana = 10; // 新增這行
    
    
    onMount(async () => {
        const response = await fetch('/json/output.json');
        gameData = await response.json();

        console.log(gameData);

        // Set the initial values after the data is fetched
        currentScene = gameData.scene.forest;
        currentCharacter = gameData.character.mage;
        currentItem = gameData.item.magic_wand;
        dialogueKeys = Object.keys(gameData.dialogue); // Get the keys of the dialogue object
        currentDialogue = gameData.dialogue[dialogueKeys[currentDialogueIndex]];

    });
    //------------------- HANDLE DATA -------------------
    function nextDialogue() {
        currentDialogueIndex++;
        intervalId = setInterval(() => {
            if (currentDialogueIndex < dialogueKeys.length) {
                currentDialogue = gameData.dialogue[dialogueKeys[currentDialogueIndex]];
            } else {
                clearInterval(intervalId);
                // Reset the dialogue index or do something else when all dialogues have been shown
                currentDialogueIndex = 0;
            }
        }, 100);
    }

    function startGame() {
        if (playerName != null && playerName != '') {
            const playerData = { name: playerName };
            localStorage.setItem('playerData', JSON.stringify(playerData));
            showContainer = false;
        }
    }




</script>

<head>
    <link rel="stylesheet" href="src/routes/start.css">
</head>

<!-- on:click={handleClick}  -->
<div     
    on:keydown={handleKeydown}
    role="button" 
    tabindex="0"
>
    {#if showContainer}
    <div class="container">
        <input bind:value={playerName} placeholder="Please enter player name" />
        <button on:click={startGame}>Start</button>
    </div>
    {/if}
    {#if !showContainer}
    <div class="scene">
        <img src={currentScene.background} alt={currentScene.name} />
    </div>
    <div class="event">
    </div>
    <div class="character-mana">Mana: { mana }</div> <!-- 新增這行 -->
    <div class="character-backpack"> <!-- 新增這行 -->
        Backpack: { backpack }
        <div class="item">
            <img src={currentItem.icon} alt={currentItem.name} />
        </div>
    </div>
    <div class="dialogue-box">
        <div class="character-info"> <!-- Add this line -->
            <div class="character-avatar">
                <img src={currentCharacter.avatar} alt={currentCharacter.name} />
            </div>
            <div class="character-name">{ playerName }</div>
        </div> <!-- Add this line -->
        <div class="character-dialogue">
            <p>{currentDialogue.text}</p>
        </div>
    </div>
    {/if}
</div>
