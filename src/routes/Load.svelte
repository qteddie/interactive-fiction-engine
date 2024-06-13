<script>
    import { navigate } from 'svelte-routing';
    import { gameState } from './store.js'; // 導入 gameState
    let isGameLoaded = false;
    let isStoryLoaded = false;
    let noGameSavedProgress = false;
    let noStorySavedProgress = false;
    let loadedGameState = null; 
    let loadedStoryState = null;
    let loadedGameTime = null;  
    let loadedStoryTime = null; 
    let loadedGamePlayerName = null;
    let loadedStoryPlayerName = null;

    function loadGame() {
        const savedProgress = localStorage.getItem('game');
        if (savedProgress) {
            const savedData = JSON.parse(savedProgress);
            loadedGameState = savedData; // 讀取遊戲進度
            loadedGameTime = new Date(savedData.saveTime).toLocaleString(); 
            loadedGamePlayerName = savedData.playerName; // 讀取玩家名稱
            isGameLoaded = true;
            noGameSavedProgress = false;
            console.log('loadedGameState',loadedGameState);
            console.log('gameState',gameState);
        } else {
            noGameSavedProgress = true;
            isGameLoaded = false;
            console.error('No saved progress found for game');
        }
    }

    function loadStory() {
        const savedProgress = localStorage.getItem('story');
        if (savedProgress) {
            const savedData = JSON.parse(savedProgress);
            loadedStoryState = savedData; // 讀取故事進度
            loadedStoryTime = new Date(savedData.saveTime).toLocaleString();
            loadedStoryPlayerName = savedData.playerName; // 讀取玩家名稱
            isStoryLoaded = true;
            noStorySavedProgress = false;
            console.log('loadedStoryState',loadedStoryState);
            console.log('gameState',gameState);
        } else {
            noStorySavedProgress = true;
            isStoryLoaded = false;
            console.error('No saved progress found for story');
        }
    }

    function startLoadedGame() {
        if (loadedGameState) {
            gameState.set(loadedGameState); 
            navigate('/start');
        } else {
            console.error('No loaded game state found');
        }
    }

    function startLoadedStory() {
        if (loadedStoryState) {
            gameState.set(loadedStoryState);
            navigate('/story');
        } else {
            console.error('No loaded story state found');
        }
    }
</script>

<head>
    <link rel="stylesheet" href="src/routes/load.css">
</head>

<div class="content">
    <button on:click={loadGame}>讀取範例遊戲存檔</button>
    {#if isGameLoaded}
        <div class="load-message">已讀取遊戲存檔</div>
        <div class="load-time">存檔時間：{loadedGameTime}</div> 
        <div class="load-player-name">玩家名稱：{loadedGamePlayerName}</div> 
        <button on:click={startLoadedGame}>開始已讀取的遊戲</button>
    {/if}
    {#if noGameSavedProgress}
        <div class="load-message">沒有遊戲存檔</div>
    {/if}

    <button on:click={loadStory}>讀取故事存檔</button>
    {#if isStoryLoaded}
        <div class="load-message">已讀取故事存檔</div>
        <div class="load-time">存檔時間：{loadedStoryTime}</div> 
        <div class="load-player-name">玩家名稱：{loadedStoryPlayerName}</div> 
        <button on:click={startLoadedStory}>開始已讀取的故事</button>
    {/if}
    {#if noStorySavedProgress}
        <div class="load-message">沒有故事存檔</div>
    {/if}
</div>