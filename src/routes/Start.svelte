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

<!-- 
<div class="scene-container">
    <img class="scene-image" src={currentScene.image} alt={currentScene.description} />
    <img class="character-avatar" src={player.avatar} alt={player.name} />
</div>
<div class="subtitle-window">
    <h2>{player.name}</h2>
    <p>{currentScene.description}</p>
</div>
<div class="item-container">
    {#each items as item (item.id)}
        <div class="item">
            <img src={item.icon} alt={item.name} />
            <p>{item.name}</p>
        </div>
    {/each}
</div> -->