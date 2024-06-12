<script>
    import { navigate } from 'svelte-routing';
    import { gameState } from './store.js'; // 導入 gameState
    let isLoaded = false;
    let noSavedProgress = false;
    let loadedGameState = null; // 定義 loadedGameState 變數
    let loadedGameTime = null;  // 新增一個變數來儲存讀取的遊戲進度
    let loadedPlayerName = null;

    function loadGame() {
        const savedProgress = localStorage.getItem('game');
        if (savedProgress) {
            const savedData = JSON.parse(savedProgress);
            loadedGameState = savedData; // 讀取遊戲進度
            loadedGameTime = new Date(savedData.saveTime).toLocaleString(); // 讀取遊戲時間並轉換為本地時間格式
            loadedPlayerName = savedData.playerName; // 讀取玩家名稱
            isLoaded = true;
            console.log('loadedGameState',loadedGameState);
            console.log('gameState',gameState);
        } else {
            noSavedProgress = true;
            console.error('No saved progress found');
        }
    }

    function startLoadedGame() {
        if (loadedGameState) {
            gameState.set(loadedGameState); // 將 gameState 更新為 loadedGameState
            navigate('/start');
        } else {
            console.error('No loaded game state found');
        }
    }
</script>

<head>
    <link rel="stylesheet" href="src/routes/load.css">
</head>

<div class="content">
    <button on:click={loadGame}>讀檔</button>
    {#if isLoaded}
        <div class="load-message">已讀取存檔</div>
        <div class="load-time">存檔時間：{loadedGameTime}</div> <!-- 顯示存檔時間 -->
        <div class="load-player-name">玩家名稱：{loadedPlayerName}</div> <!-- 顯示玩家名稱 -->
        <button on:click={startLoadedGame}>開始已讀取的遊戲</button>
    {/if}
    {#if noSavedProgress}
        <div class="load-message">沒有存檔</div>
    {/if}
</div>