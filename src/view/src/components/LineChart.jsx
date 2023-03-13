import React from 'react';
import { Chart } from 'chart.js/auto'
import { Line } from 'react-chartjs-2';

//Chart.register(CategoryScale);

export const LineChart = ( { text, labels, data } ) => {
    
const state = {
  labels: labels,
  datasets: [
    {
      label: 'Rainfall',
      fill: false,
      lineTension: 0.2,
      backgroundColor: 'rgba(75,192,192,1)',
      borderColor: 'rgba(255, 255, 255, 0.87)',
      borderWidth: 1,
      data: data
    }
  ]
}
    return (
      <>
        <div>
          <Line
            data={state}
            options={
              {
              responsive: true,
              title:{
                display:true,
                text:text,
                fontSize:20
              },
              legend:{
                display:true,
                position:'right'
              }
            }}
          />
        </div>
      </>
    );
}
