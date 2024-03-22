<script setup lang="ts">
import {ref} from 'vue'
import MainBody from './components/MainBody.vue'

const windowWidth = ref(window.innerWidth)
const windowHeight = ref(window.innerHeight)

const onResize = (e: UIEvent) => {
  console.log(e);
  windowWidth.value = window.innerWidth;
  windowHeight.value = window.innerHeight;
}
window.onresize = onResize

</script>

<template>
  <header>
    <div>
      <div v-text="'w:' + windowWidth"/>
      <div v-text="'h:' + windowHeight"/>
    </div>
  </header>

  <main>
    <div class="wrapper">
      <MainBody msg="You did it!" />
    </div>
  </main>
</template>

<style scoped>
header {
  line-height: 1.5;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

@media (min-width: 1024px) {
  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }

  .logo {
    margin: 0 2rem 0 0;
  }

  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }
}
</style>
