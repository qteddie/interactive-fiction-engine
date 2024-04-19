<script>
    let scenes = [
        {
            id: 'classroom',
            description: 'You are in a dark room. There are two doors.',
            image: 'scenes/classroom.jpg',
            choices: [
                { text: 'Left door', nextScene: 'treasure' },
                { text: 'Right door', nextScene: 'pit' },
            ],
        },
        // 更多場景...
    ];

    let characters = [
        {
            id: 'player',
            name: 'frieren',
            avatar: 'characters/frieren.png',
            items: [],
            // 更多角色屬性...
        },
        // 更多角色...
    ];

    let items = [
        {
            id: 'key',
            name: 'Key',
            icon: '/path/to/key/icon.jpg',
            // 更多物品屬性...
        },
        // 更多物品...
    ];

    let currentScene = scenes[0];
    let player = characters.find(character => character.id === 'player');

    function makeChoice(choice) {
        currentScene = scenes.find(scene => scene.id === choice.nextScene);
    }
</script>

<head>
    <link rel="stylesheet" href="src/styles.css">
</head>

<div class="scene-container">
    <img class="scene-image" src={currentScene.image} alt={currentScene.description} />
    <img class="character-avatar" src={player.avatar} alt={player.name} />
</div>
<div class="subtitle-window">
    <p>{currentScene.description}</p>
</div>