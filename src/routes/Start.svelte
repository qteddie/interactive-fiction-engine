<!-- src/routes/Start.svelte -->
<script>
    
    let playerName = '';
    let showContainer = true;

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

{#if showContainer}
<div class="container">
    <input bind:value={playerName} placeholder="Please enter player name" />
    <button on:click={startGame}>Start</button>
</div>
{/if}