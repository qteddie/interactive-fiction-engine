<script>
    import { pushState, replaceState } from '$app/navigation';

    // Use pushState to add a new entry to the history stack
    // pushState('/', { replace: true, keepfocus: true });
    //------------------- HANDLE DATA -------------------
    import { onMount, onDestroy } from 'svelte';
    import { navigate } from 'svelte-routing';
    import { gameState, music } from './store.js';

    let isTransitioning = false;
    



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
    let currentOptions = []; // Add this line
    let showEndScreen = false;
    let audio;

    onMount(async () => {
        const response = await fetch('/json/output_option2.json');
        gameData = await response.json();

        console.log(gameData);

        // Set the initial values after the data is fetched
        if ($gameState) { // 如果 gameState 的值不為 null
            currentScene = $gameState.currentScene; // 使用 gameState 的值設定當前場景
            currentCharacter = $gameState.currentCharacter; // 使用 gameState 的值設定當前角色
            currentItem = $gameState.currentItem; // 使用 gameState 的值設定當前物品
            currentDialogueIndex = $gameState.currentDialogueIndex; // 使用 gameState 的值設定當前對話索引
            playerName = $gameState.playerName; // 使用 gameState 的值設定玩家名稱
            mana = $gameState.mana; // 使用 gameState 的值設定當前 mana
            backpack = $gameState.backpack; // 使用 gameState 的值設定背包
            showEndScreen = $gameState.showEndScreen; // 使用 gameState 的值設定是否顯示結束畫面
            showContainer = false; // 使用 gameState 的值設定是否顯示開始畫面
        } else {
            currentScene = gameData.scene.forest;
            currentCharacter = gameData.character.mage;
            currentItem = gameData.item.magic_wand;
        }

        dialogueKeys = Object.keys(gameData.dialogue); // Get the keys of the dialogue object
        currentDialogue = gameData.dialogue[dialogueKeys[currentDialogueIndex]]; // 根據 currentDialogueIndex 設定 currentDialogue
        currentOptions = gameData.dialogue[dialogueKeys[currentDialogueIndex]].options; // 根據 currentDialogueIndex 設定 currentOptions
    });
// ------------------------ Music ------------------------
    let musicFiles = ['1.mp3', '2.mp3', '3.mp3']; // 你的音樂檔名

    onMount(() => {
        musicFiles.forEach(file => {
            let audio = new Audio(`/music/bgm/${file}`);
            audio.loop = true;
            // 將音樂添加到 music store 中
            music.update(musicArray => [...musicArray, audio]);
        });
        // 播放第一首音樂
        music.update(musicArray => {
            musicArray[1].play();
            return musicArray;
        });
    });

    onDestroy(() => {
        // 停止所有正在播放的音樂
        music.update(musicArray => {
            for (let i = 0; i < musicArray.length; i++) {
                musicArray[i].pause();
            }
            return musicArray;
        });
    });

    onDestroy(() => {
        // 自動存檔
        saveGame();
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
            currentOptions,
            showEndScreen
        };
        try {
            console.log('Game saved');
            localStorage.setItem('game', JSON.stringify(game));
        } catch (error) {
            console.error('Error saving to localStorage', error);
        }
        isSave = true;

        setTimeout(() => {
            isSave = false;
        }, 1000);
    }

    function endGame() {
        // gameState = null; // 將 gameState 設定為 null
        localStorage.removeItem('game');
        showEndScreen = true; 

    }
    function goToStartScreen() {
        showEndScreen = false;
        navigate('/');
    }

    function nextDialogue() {
        currentDialogueIndex++;
        intervalId = setInterval(() => {
            if (currentDialogueIndex < dialogueKeys.length) {
                currentDialogue = gameData.dialogue[dialogueKeys[currentDialogueIndex]];
                // 檢查當前對話是否有事件
                if (currentDialogue.options && currentDialogue.options.event) {
                    const event = gameData.event[currentDialogue.options.event];
                    if (event) {
                        isTransitioning = true;
                        setTimeout(() => {
                            currentScene = gameData.scene[event.scene];
                            isTransitioning = false;
                        }, 1000); // Wait for 1 second before changing the scene
                    }
                }
                // 查找對應的角色數據
                const character = gameData.character[currentDialogue.character];
                if (character) {
                    // 更新頭像和立繪
                    currentCharacter = character;
                }
            } else {
                endGame();
                // Reset the dialogue index or do something else when all dialogues have been shown
            }
        }, 100);
    }

    function startGame() {
        if (playerName != null && playerName != '') {
            const playerData = { name: playerName };
            localStorage.setItem('playerData', JSON.stringify(playerData));
            showContainer = false;
            isTransitioning = true; // 新增這行
            setTimeout(() => { // 新增這行
                showContainer = false;
                isTransitioning = false; // 新增這行
            }, 1000); // 新增這行
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
    <div class="character-mana" style="background: linear-gradient(to right, #00f 0%, #00f {mana * 10}%, #fff {mana * 10}%, #fff 100%)">Mana: { mana }</div>    
    <div class="character-backpack"> 
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
    <div class="save-button">
        <button on:click={saveGame}>save</button>
    </div>

    {/if}
</div>