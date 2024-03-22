<script setup lang="ts">
import { Scatter } from 'vue-chartjs'
import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  PointElement,
  LineElement,
  LinearScale
} from 'chart.js'

ChartJS.register(Title, Tooltip, Legend, PointElement, LineElement, LinearScale)

const chartData = {
  datasets: [
    {
      label: 'Scatter Dataset',
      data: [
        { x: -10, y: 0 },
        { x: 0, y: 10 },
        { x: 10, y: 5 },
        { x: 0.5, y: 5.5 }
      ],
      //   showLine: true,
      borderWidth: 3,
      borderColor: 'red',
      backgroundColor: '#0f0'
    }
  ]
}

const chartOptions = {
  responsive: true
}
</script>

<template>
  <Scatter id="my-chart-scatter" :data="chartData" :options="chartOptions" />
</template>
