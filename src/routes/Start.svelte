<script>
    
    import { pushState, replaceState } from '$app/navigation';

    // Use pushState to add a new entry to the history stack
    // pushState('/', { replace: true, keepfocus: true });
    //------------------- HANDLE DATA -------------------
    import { onMount, onDestroy } from 'svelte';
    import { navigate } from 'svelte-routing';
    import { gameState} from './store.js';

    let isTransitioning = false;
    // ------------------------------- EMCC START ------------------------------

    let Module = {};

    function stringToUTF8(str, ptr) {
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        for (let i = 0; i < str.length; i++) {
            data[i] = str.charCodeAt(i);
        }
        data[str.length] = 0;  // Null-terminate the string
    }

    function UTF8ToString(ptr) {
        const memory = Module.memory || importObject.env.memory;
        const data = new Uint8Array(memory.buffer, ptr);
        let length = 0;
        while (data[length]) length++;
        return new TextDecoder('utf-8').decode(data.subarray(0, length));
    }

    function emscripten_resize_heap(requestedSize) {
        throw new Error('Heap resize is not supported');
    }

    const importObject = {
        env: {
            memory: new WebAssembly.Memory({ initial: 256, maximum: 2048 }),
            table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' }),
            __wbindgen_add_to_stack_pointer: () => {return 0; },
            emscripten_memcpy_js: function(dest, src, num) {
                const mem = new Uint8Array(this.memory.buffer);
                if (src + num > mem.length || dest + num > mem.length) {
                    throw new Error("Attempt to access memory outside buffer bounds");
                }
                mem.set(mem.subarray(src, src + num), dest);
            },
            emscripten_resize_heap: emscripten_resize_heap,
        },    
    };

    function processAndLogString(inputString) {
        const inputPtr = Module.malloc(inputString.length + 1); // Allocate memory for string
        stringToUTF8(inputString, inputPtr);
        const ptr = Module.processData(inputPtr);
        const result = UTF8ToString(ptr);
        // console.log('Processed data:', UTF8ToString(ptr));
        Module.free(inputPtr);
        return result;
    }

    // function getNextString(inputString) {
    //     const inputPtr = Module.malloc(inputString.length + 1); // Allocate memory for string
    //     stringToUTF8(inputString, inputPtr);
    //     const ptr = Module.getNextEncounter(inputPtr);
    //     // const result = UTF8ToString(ptr);
    //     console.log('Next data:', UTF8ToString(ptr));
    //     Module.free(inputPtr);
    //     // return result;
    // }

    onMount(async () => {
        const wasmUrl = '/c/test.wasm';
        const wasmResponse = await fetch(wasmUrl);
        const { instance } = await WebAssembly.instantiateStreaming(wasmResponse, importObject);
        Module = instance.exports;

        if (!Module.memory) {
            Module.memory = importObject.env.memory;
        }
        
        console.log('WebAssembly module loaded and ready.');
        // getNextString("{'text': '2e�!', 'next': 'dragon_encounter'}");
        // processAndLogString("28749817834");
    });

    // ------------------------------- EMCC END ------------------------------
    let divElement;

    function handleKeydown(event) {
    if (event.key === ' ') {
        if (currentDialogue.options && currentDialogue.options.length > 1) {
            return;
        }
        nextDialogue();
    }
}
    // function handleClick() {
    //     // window.alert('Hello');
    //     nextDialogue();
    // }

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
    
    let displayText = '';
    let displayIndex = 0;
    let typingSpeed = 50; // 调整显示速度，单位为毫秒

    onMount(async () => {
        const response = await fetch('/json/output3.json');
        gameData = await response.json();
        // console.log('gameState before: ',$gameState);
        
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
            // console.log('gameState after clear: ',$gameState);
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
        showDialogue(currentDialogue.text);
    });
// ------------------------ Music ------------------------

    // onDestroy(() => {
    //     // 自動存檔
    //     saveGame();
    // });
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
        
        
        if (option) {
            if(option.next){
                const nextString = processAndLogString(option.next);
                // console.log('nextString: ',nextString);

                currentDialogue = gameData.dialogue[nextString];
                currentDialogueIndex = dialogueKeys.indexOf(nextString);
                }
                else if (option.event) {
                    const eventString = processAndLogString(option.event);
                    // console.log('eventString: ',eventString);

                    // console.log('option.event: ',option.event);

                const event = gameData.event[eventString];
                // console.log('event: ',event);
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
            else if (!option.next && !option.event)
            {
                endGame();
                return;
            }
        } 

        const character = gameData.character[currentDialogue.character];
        if (character) {
            // 更新頭像和立繪
            currentCharacter = character;
        }
        showDialogue(currentDialogue.text);
    }

    // --------------------------------- DIALOGUE ---------------------------------
    function showDialogue(text) {
        displayText = '';
        displayIndex = 0;

        intervalId = setInterval(() => {
            if (displayIndex < text.length) {
                displayText += text.charAt(displayIndex);
                displayIndex++;
            } else {
                clearInterval(intervalId);
            }
        }, typingSpeed);
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
        <div class="character-info">
            <div class="character-avatar">
                <img src={currentCharacter.avatar} alt={currentCharacter.name} />
            </div>
            <div class="character-name">{ playerName }</div>
        </div>
        <div class="character-dialogue">
            <p>{displayText}</p>
            {#if Array.isArray(currentDialogue.options)}
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