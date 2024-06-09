<script>
    import { navigate } from 'svelte-routing';
    import { gameState } from './store.js'; // 導入 gameState
    let isLoaded = false;
    let noSavedProgress = false;
    function loadGame() {
        const savedProgress = localStorage.getItem('game');
        if (savedProgress) {
            gameState.set(JSON.parse(savedProgress)); // 更新 gameState 的值
            isLoaded = true;

            console.log($gameState);
            navigate('/start');
        } else {
            noSavedProgress = true;
            console.error('No saved progress found');
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
    {/if}
    {#if noSavedProgress}
        <div class="load-message">沒有存檔</div>
    {/if}
</div>