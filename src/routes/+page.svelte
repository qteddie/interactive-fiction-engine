<!-- src/+page.svelte -->
<script>
    import { navigate } from 'svelte-routing';
    import Router from './Router.svelte';
    let gameStarted = false;
    let is_Test = false;
    function startGame() {
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
    function openTest() {
        navigate('/test');
    }

</script>

<Router />

<head>
    <link rel="stylesheet" href="src/routes/+page.css">
</head>
<div class="header">
    <h1>Interactive-Fiction-engine</h1>
</div>
<div class="menu">
    <button on:click={startGame}>Start</button>
    <button on:click={loadGame}>Load</button>
    <button on:click={openSettings}>Settings</button>
    <button on:click={openHelp}>Help</button>
    <button on:click={openTest}>Test</button>
</div>