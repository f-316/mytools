<script setup lang="ts">
import {ref, computed} from 'vue'
import {TheSample} from './Sample'
defineProps<{
  msg: string
}>()
const cnt = ref(0)
setInterval(() => {
  cnt.value++
}, 1000)
</script>

<template>
  <div class="greetings">
    <h1 class="green">{{ cnt }}</h1>
    <TheSample/>
  </div>
</template>

<style scoped>
h1 {
  font-weight: 500;
  font-size: 2.6rem;
  position: relative;
  top: -10px;
}

h3 {
  font-size: 1.2rem;
}

.greetings h1,
.greetings h3 {
  text-align: center;
}

@media (min-width: 1024px) {
  .greetings h1,
  .greetings h3 {
    text-align: left;
  }
}
@media (max-width: 512px) {
  .green {
    color: red;
  }
}
</style>
