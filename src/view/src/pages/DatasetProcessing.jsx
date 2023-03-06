import React, { useState, useEffect } from "react";
import { useLocation } from "react-router-dom";

import { ProgressBar } from "../components/progressBar";

import "../styles/dataset-processing.css";
import axios from "axios";


export const DatasetProcessing = () => {
  const location = useLocation();
  const [ percentage, setPercentage ] = useState( 0 );
  const [ progressText, setProgressText ] = useState( "Dataset preprocessing..." );

  useEffect(() => {
    const formData = new FormData();
    formData.append("file", location.state.file);
    formData.append("normalize_data", location.state.normalizeData);
    formData.append("map_desired_output", location.state.mapOutDesiredOutput);
    axios.post("http://127.0.0.1:8000/split-dataset", formData, {
        headers: {
          "Content-Type": "multipart/form-data",
        },
      })
      .then((response) => {
        console.log(response);
      });
  }, []);

  return (
    <>
      <div className="dataset-processing">
        <h1> { progressText } </h1>
        <br />
      </div>
      <ProgressBar value={ percentage }/>
    </>
  );
};
