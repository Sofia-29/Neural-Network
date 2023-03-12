import React, { useState, useEffect } from "react";
import { useLocation } from "react-router-dom";
import axios from "axios";

const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

export const DatasetProcessing = () => {
  const location = useLocation();
  const [response, setResponse] = useState("");
  const [progressText, setProgressText] = useState("Dataset preprocessing...");

  useEffect(() => {
    const formData = new FormData();
    formData.append("file", location.state.file);
    formData.append("normalize_data", location.state.normalizeData);
    formData.append("map_desired_output", location.state.mapOutDesiredOutput);
    formData.append("split_dataset", location.state.splitDataset);
    axios
      .post("http://127.0.0.1:8000/split-dataset", formData, {
        headers: {
          "Content-Type": "multipart/form-data",
        },
      })
      .then((results) => {
        if (results.data["response"] == "Ok") {
          const timeoutId = setTimeout(() => {
            setResponse("trainData");
          }, 2000); 
          return () => {
            clearTimeout(timeoutId);
          };
        }
      });
  }, []);

  useEffect(() => {
    if (response != "") {
      setProgressText("Training dataset, this may take a while...");
      if (response == "trainData") {
        axios.get("http://127.0.0.1:8000/train-dataset").then((results) => {
          // if (results.data["response"] == "Ok") {
            
          // }
        });
      }
    }
  }, [response]);

  return (
    <>
        <h1> {progressText} </h1>
        <br />
        <i className="fa-solid fa-rocket fa-spin fa-10x"></i>
    </>
  );
};
