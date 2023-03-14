import React from 'react';
import { Chart } from "chart.js/auto";
import { Line } from 'react-chartjs-2';

export const LineChart = ({ text, labels, data }) => {

  const dataSettings = {
    labels: labels,
    datasets: [
      {
        label: 'SSE',
        data: data,
        fill: true,
        backgroundColor: 'rgba(0, 0, 0, 0.5)',
        borderColor: 'rgba(255, 255, 255, 0.87)',
      },
    ],
  };
  
  const options = {

    plugins: {
      title: {
        display: true,
        text: text,
        fontSize: 20,
        color: 'white'
      },
      legend: {
        display: true,
        position: 'top',
        labels: {
          color: 'white',
        },
      },
    },
    scales: {
      y: {
        ticks: {
          color: 'white',
        },
        grid: {
          color: 'white',
        },
      },
      x: {
        ticks: {
          color: 'white',
        },
        grid: {
          color: 'white',
        },
      },
    },
  };

  return (
    <Line data={ dataSettings } options={ options } />
  );
};
