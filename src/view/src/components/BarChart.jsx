import React from "react";
import { Chart } from "chart.js/auto";
import { Bar } from "react-chartjs-2";

export const BarChart = ( { text, labels, datasetLabel, data } ) => {
    const options = {
        responsive: true,
        plugins: {
        title: {
            display: true,
            text: text,
            fontSize: 20,
            color: "white",
        },
        legend: {
            display: true,
            position: "top",
            labels: {
            color: "white",
            },
        },
        },
        scales: {
        y: {
            ticks: {
            color: "white",
            },
            grid: {
            color: "white",
            },
        },
        x: {
            ticks: {
            color: "white",
            },
            grid: {
            color: "white",
            },
        },
        },
  };


  const dataSettings = {
    labels,
    datasets: [
      {
        label: datasetLabel,
        data: data,
        backgroundColor: "rgba(255, 255, 255, 0.87)",
      },
    ],
  };

  return <Bar options={options} data={dataSettings} />;
};
