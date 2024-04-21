<!-- src/+page.svelte -->
<script>
    import { navigate } from 'svelte-routing';
    import Router from './Router.svelte';
    let gameStarted = false;
    function startGame() {
        gameStarted = true;
        navigate('/start');
    }

    function loadGame() {
        navigate('/load');
    }

    function openSettings() {
        navigate('/settings');
    }

    function openHelp() {
        navigate('/help');
    }
</script>

<Router />

<head>
    <link rel="stylesheet" href="src/routes/+page.css">
</head>

<div class="menu">
    {#if !gameStarted}
        <button on:click={startGame}>Start</button>
        <button on:click={loadGame}>Load</button>
        <button on:click={openSettings}>Settings</button>
        <button on:click={openHelp}>Help</button>
    {/if}
</div>