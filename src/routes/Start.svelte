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
    let wasmModule;
    let wasmLoaded = false; 

    function print_string(ptr) {
        const memory = new Uint8Array(wasmModule.memory.buffer);
        let str = "";
        for (let i = ptr; memory[i] !== 0; i++) {
            str += String.fromCharCode(memory[i]);
        }
        console.log(str);
    }
    async function emscripten_sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }
    function emscripten_memcpy_js(dest, src, num) {
        const memory = new Uint8Array(wasmModule.memory.buffer);
        memory.set(memory.subarray(src, src + num), dest);
    }
    function emscripten_resize_heap(requestedSize) {
        const memory = wasmModule.memory;
        const oldSize = memory.buffer.byteLength;
        if (requestedSize > oldSize) {
            const pagesNeeded = Math.ceil((requestedSize - oldSize) / 65536);
            try {
                memory.grow(pagesNeeded);
                return 1;
            } catch (e) {
                return 0;
            }
        }
        return 1;
    }
    onMount(async () => {
        const wasmUrl = '/c/main.wasm';
        const importObject = {
            env: {
                memory: new WebAssembly.Memory({ initial: 256 }),
                table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' }),
                emscripten_sleep,
                emscripten_memcpy_js,
                emscripten_resize_heap,
                print_string: print_string
            }
        };
        const wasmResponse = await fetch(wasmUrl);
        const { instance } = await WebAssembly.instantiateStreaming(wasmResponse, importObject);
        wasmModule = instance.exports;
        wasmLoaded = true;  // 设置加载标志
    });

    // ------------------------------- EMCC END ------------------------------

    // "mage": {
    //         "name": "Mage (you)",
    //         "avatar": ["/character/mage/avatar.jpeg", "/character/mage/avatar2.jpeg", "/character/mage/avatar3.jpeg"],
    //         "tachie": ["/character/mage/tachie3.png", "/character/mage/tachie2.png", "/character/mage/tachie.png"]
    //     },

    // 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

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
        if (!wasmLoaded) {  // 检查WebAssembly模块是否已加载
            console.error("WebAssembly module not loaded yet.");
            return;
        }

        clearInterval(intervalId); // 清除 intervalId

        let optionNext = '';
        let optionEvent = '';

        if (option) {
            if (option.next) {
                optionNext = option.next;
            }
            if (option.event) {
                optionEvent = option.event;
            }
        } else {
            currentDialogue.options.forEach(opt => {
                if (opt.next) {
                    optionNext = opt.next;
                }
                if (opt.event) {
                    optionEvent = opt.event;
                }
            });
        }
        console.log('optionNext:', optionNext);
        console.log('optionEvent:', optionEvent);

        wasmModule.nextDialogue(optionNext, optionEvent);
        console.log('Called nextDialogue with:', optionNext, optionEvent);

        // 打印最新的 currentDialogue 值
        const memory = new Uint8Array(wasmModule.memory.buffer);
        const textPtr = wasmModule.getCurrentDialogueText();
        console.log('textPtr:', textPtr);

        // 确认 textPtr 是否有效
        if (!textPtr) {
            console.error('Invalid textPtr:', textPtr);
            return;
        }

        // 打印 memory 数组中的部分内容以确认内存是否正确分配
        console.log('Memory slice:', memory.slice(textPtr, textPtr + 100));

        const text = [];
        for (let i = textPtr; memory[i] !== 0; i++) {
            text.push(String.fromCharCode(memory[i]));
        }
        console.log('Updated currentDialogue.text:', text.join(''));
        showDialogue(text.join(''));

        // 如果存在事件，进行场景转换
        if (optionEvent) {
            const eventIndex = gameData.events.findIndex(event => event.dialogue === optionEvent);
            if (eventIndex !== -1) {
                isTransitioning = true;
                setTimeout(() => {
                    currentScene = gameData.scenes[eventIndex];
                    isTransitioning = false;
                }, 1000); // 等待 1 秒钟后再改变场景
            }
        }

        // 检查并设置当前角色
        const character = gameData.character[currentDialogue.character];
        if (character) {
            currentCharacter = character;
        }

        // 检查当前对话是否有选项，如果没有，结束游戏
        if (!currentDialogue.options.length) {
            endGame();
            return;
        }
    }
    // }
    // function nextDialogue(option) {
    //     clearInterval(intervalId);

    //     if (option) {
    //         if(option.next){
    //             currentDialogue = gameData.dialogue[option.next];
    //             currentDialogueIndex = dialogueKeys.indexOf(option.next);
    //         }
    //         if (option.event) {
    //             const event = gameData.event[option.event];
    //             currentDialogue = gameData.dialogue[event.dialogue];
    //             currentDialogueIndex = dialogueKeys.indexOf(event.dialogue);
    //             if (event) {
    //                 isTransitioning = true;
    //                 setTimeout(() => {
    //                     currentScene = gameData.scene[event.scene];
    //                     isTransitioning = false;
    //                 }, 1000); // Wait for 1 second before changing the scene
    //             }
    //         }
    //     } else {
    //         currentDialogue.options.forEach(option => {
    //             if (option.next) {
    //                 if (!gameData.dialogue.hasOwnProperty(option.next)) {
    //                     console.error(`Error: Dialogue "${option.next}" does not exist.`);
    //                     endGame();
    //                     return;
    //                 }
    //                 currentDialogue = gameData.dialogue[option.next];
    //                 currentDialogueIndex = dialogueKeys.indexOf(option.next);
    //             }
    //             if (option.event) {
    //                 const event = gameData.event[option.event];
    //                 if (event) {
    //                     isTransitioning = true;
    //                     setTimeout(() => {
    //                         currentScene = gameData.scene[event.scene];
    //                         isTransitioning = false;
    //                     }, 1000); // Wait for 1 second before changing the scene
    //                 }
    //             }
    //         });
    //     }

    //     if (!currentDialogue.options) {
    //         endGame();
    //         return;
    //     }

    //     const character = gameData.character[currentDialogue.character];
    //     if (character) {
    //         currentCharacter = character;
    //     }

    //     showDialogue(currentDialogue.text);
    // }


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