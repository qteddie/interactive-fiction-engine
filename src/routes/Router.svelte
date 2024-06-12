<!-- src/Router.svelte -->
<script>
    import { Router, Route } from 'svelte-routing';
    import Start from './Start.svelte';
    import Load from './Load.svelte';
    import Settings from './Settings.svelte';
    import Help from './Help.svelte';
    import Test from './Test.svelte'; // Add this line
    import Story from './Story.svelte'; // Add this line

    // import 'github-markdown-css/github-markdown.css';
</script>

<svelte:head>
    <link rel="stylesheet" href="/github-markdown-css/github-markdown.css">
</svelte:head>
<Router>
    <Route path="/start" component={Start} />
    <Route path="/story" component={Story} />
    <Route path="/load" component={Load} />
    <Route path="/settings" component={Settings} />
    <Route path="/help" component={Help} />
    <Route path="/test" component={Test} /> <!-- Add this line -->

</Router>