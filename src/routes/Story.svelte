<script>
    
    import { pushState, replaceState } from '$app/navigation';

    // Use pushState to add a new entry to the history stack
    // pushState('/', { replace: true, keepfocus: true });
    //------------------- HANDLE DATA -------------------
    import { onMount, onDestroy } from 'svelte';
    import { navigate } from 'svelte-routing';
    import { gameState} from './store.js';

    let isTransitioning = false;

    let divElement;

    function handleKeydown(event) {
    if (event.key === ' ') {
        if (currentDialogue.options && currentDialogue.options.length > 1) {
            return;
        }
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
    let showEndScreen = false;
    let audio;

    onMount(async () => {
        const response = await fetch('/json/output3.json');
        gameData = await response.json();
        console.log('gameState before: ',$gameState);
        
        // 如果 gameState 的值不為 null，則表示從 startLoadedGame 導航過來
        if ($gameState) {
            currentScene = $gameState.currentScene; // 使用 gameState 的值設定當前場景
            currentCharacter = $gameState.currentCharacter; // 使用 gameState 的值設定當前角色
            currentDialogueIndex = $gameState.currentDialogueIndex; // 使用 gameState 的值設定當前對話索引
            playerName = $gameState.playerName; // 使用 gameState 的值設定玩家名稱
            mana = $gameState.mana; // 使用 gameState 的值設定當前 mana
            backpack = $gameState.backpack; // 使用 gameState 的值設定背包
            showEndScreen = $gameState.showEndScreen; // 使用 gameState 的值設定是否顯示結束畫面
            showContainer = false; // 使用 gameState 的值設定是否顯示開始畫面
            $gameState = null; // 清空 $gameState
            console.log('gameState after clear: ',$gameState);
        } else {
            // 如果 gameState 的值為 null，則表示需要重新開始遊戲
            currentScene = gameData.scene.forest;
            currentCharacter = gameData.character.mage;
            currentDialogueIndex = 0; // 將對話索引重設為 0
            playerName = ''; // 將玩家名稱重設為空字串
            mana = 10; // 將 mana 重設為 100
            backpack = gameData.item; // 將背包重設為初始物品
            showEndScreen = false; // 將是否顯示結束畫面重設為 false
            showContainer = true; // 將是否顯示開始畫面重設為 true
        }

        dialogueKeys = Object.keys(gameData.dialogue); // Get the keys of the dialogue object
        currentDialogue = gameData.dialogue[dialogueKeys[currentDialogueIndex]]; // 根據 currentDialogueIndex 設定 currentDialogue
    });

    //------------------- HANDLE DATA -------------------
    let isSave = false;
    function saveGame() {
        const game = {
            currentScene,
            currentCharacter,
            currentItem,
            currentDialogueIndex,
            playerName,
            backpack,
            mana,
            showEndScreen,
            showContainer,
            saveTime: new Date().toISOString(), // 新增存檔時間
        };
        try {
            console.log('Game saved');
            localStorage.setItem('game', JSON.stringify(game));
            console.log(game);
        } catch (error) {
            console.error('Error saving to file', error);
        }
        isSave = true;

        setTimeout(() => {
            isSave = false;
        }, 1000);
    }
    let initialGameState = {
        currentScene: null,
        currentCharacter: null,
        currentItem: null,
        currentDialogueIndex: 0,
        playerName: '',
        backpack: {},
        mana: 10,
        showEndScreen: false,
    };
    function endGame() {
        localStorage.removeItem('game');
        // 重置遊戲存檔
        localStorage.setItem('game', JSON.stringify(initialGameState));
        showEndScreen = true;
    }
    function goToStartScreen() {
        showEndScreen = false;
        navigate('/');
        }
    function startGame() {
        if (playerName != null && playerName != '') {
            const playerData = { name: playerName };
            localStorage.setItem('playerData', JSON.stringify(playerData));
            showContainer = false;
        }
    }
    function nextDialogue(option) {
        clearInterval(intervalId);
        // console.log(option);
        // console.log(currentDialogue.options);
        if (option) {
            // if (!gameData.dialogue.hasOwnProperty(option.next)) {
            //     console.error(`Error: Dialogue "${option.next}" does not exist.`);
            //     endGame();
            //     return;
            // }
            if(option.next){
                // console.log(option.next);
                currentDialogue = gameData.dialogue[option.next];
                currentDialogueIndex = dialogueKeys.indexOf(option.next);
            }
            if (option.event) {
                // console.log(option.event);
                const event = gameData.event[option.event];
                // console.log(event);
                currentDialogue = gameData.dialogue[event.dialogue];
                currentDialogueIndex = dialogueKeys.indexOf(event.dialogue);
                if (event) {
                    isTransitioning = true;
                    setTimeout(() => {
                        currentScene = gameData.scene[event.scene];
                        isTransitioning = false;
                    }, 1000); // Wait for 1 second before changing the scene
                }
            }
        }
        else {
            currentDialogue.options.forEach(option => {
                if (option.next) {
                    if (!gameData.dialogue.hasOwnProperty(option.next)) {
                        console.error(`Error: Dialogue "${option.next}" does not exist.`);
                        endGame();
                        return;
                    }
                    currentDialogue = gameData.dialogue[option.next];
                    // 更新 currentDialogueIndex 以指向 option.next 對應的對話
                    currentDialogueIndex = dialogueKeys.indexOf(option.next);
                }
                if (option.event) {
                    const event = gameData.event[option.event];
                    if (event) {
                        isTransitioning = true;
                        setTimeout(() => {
                            currentScene = gameData.scene[event.scene];
                            isTransitioning = false;
                        }, 1000); // Wait for 1 second before changing the scene
                    }
                }
            });
        }
        if (!currentDialogue.options) {
            endGame();
            return;
        }

        // 查找對應的角色數據
        const character = gameData.character[currentDialogue.character];
        if (character) {
            // 更新頭像和立繪
            currentCharacter = character;
        }
    }

    
</script>

<head>
    <link rel="stylesheet" href="src/routes/start.css">
</head>

<!-- on:click={handleClick}  -->

{#if isSave}
    <div class="save-message">
        <h1>已存檔</h1>
    </div>
{/if}

{#if showEndScreen}
    <div class="end-screen">
        <h1>遊戲結束</h1>
        <button on:click={goToStartScreen}>回到主畫面</button>
    </div>
{/if}
{#if showContainer}
<div class="container">
    <input bind:value={playerName} placeholder="Please enter player name" />
    <button on:click={startGame}>Start</button>
</div>
{/if}


<div id="transition-overlay" class={isTransitioning ? 'active' : ''}></div> 
<div     
    on:keydown={handleKeydown}
    role="button" 
    tabindex="0"
>
    {#if !showContainer}
    <div class="scene">
        <img src={currentScene.background} alt={currentScene.name} />
    </div>
    <div class="character-tachie"> 
        <img src={currentCharacter.tachie} alt={currentCharacter.name} />
    </div> 
    <div class="event">
    </div>
    <div class="character-mana" style="background: linear-gradient(to right, rgba(255, 255, 255, 0.5) 0%, rgba(255, 255, 255, 0.5) {mana * 10}%, rgba(255, 255, 255, 0.5) {mana * 10}%, rgba(255, 255, 255, 0.5) 100%)">Mana: { mana }</div>
    <div class="character-backpack">
        {#each Object.values(backpack) as item (item.name)}
          <div class="item">
            <img src={item.icon} alt={item.name} />
          </div>
        {/each}
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
            {#if Array.isArray(currentDialogue.options) && currentDialogue.options.length > 1}
                <div class="dialogue-options">
                    {#each currentDialogue.options as option (option.text)}
                        <button on:click={() => nextDialogue(option)}>{option.text}</button>
                    {/each}
                </div>
            {/if}
        </div>
    </div>
    <div class="save-button">
        <button on:click={saveGame}>save</button>
    </div>

    {/if}
</div>