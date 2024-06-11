<!-- src/+page.svelte -->
<script>
    import { navigate } from 'svelte-routing';
    import Router from './Router.svelte';
    import { onMount, onDestroy } from 'svelte';
    import { music } from './store.js';
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


    let musicFiles = ['1.mp3', '2.mp3', '3.mp3', '4.mp3', '5.mp3']; // 你的音樂檔名
    let currentMusicIndex = 0;

    onMount(() => {
        musicFiles.forEach((file, index) => {
            let audio = new Audio(`/music/bgm/${file}`);
            audio.loop = false; // 不再讓每首音樂循環播放
            audio.onended = () => {
                // 當音樂結束時，播放下一首音樂
                currentMusicIndex = (currentMusicIndex + 1) % musicFiles.length;
                music.update(musicArray => {
                    musicArray[currentMusicIndex].play();
                    return musicArray;
                });
            };
            // 將音樂添加到 music store 中
            music.update(musicArray => [...musicArray, audio]);
        });
        // 播放第一首音樂
        music.update(musicArray => {
            musicArray[currentMusicIndex].play();
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