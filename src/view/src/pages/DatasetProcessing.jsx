import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { useLocation } from "react-router-dom";
import axios from "axios";

export const DatasetProcessing = () => {
  const location = useLocation();
  const navigate = useNavigate();
  const [response, setResponse] = useState("");
  const [progressText, setProgressText] = useState("Dataset preprocessing...");
  const [error, setError] = useState(false);

  useEffect(() => {
    const formData = new FormData();
    formData.append("file", location.state.file);
    formData.append("normalize_data", location.state.normalizeData);
    formData.append("map_desired_output", location.state.mapOutDesiredOutput);
    formData.append("split_dataset", location.state.splitDataset);
    axios
      .post("http://127.0.0.1:8000/process-data", formData, {
        headers: {
          "Content-Type": "multipart/form-data",
        },
      })
      .then((results) => {
        console.log(results);
        if (results.data["response"] == "Ok") {
          const timeoutId = setTimeout(() => {
            setResponse("trainData");
          }, 2000);
          return () => {
            clearTimeout(timeoutId);
          };
        } else {
          setError(!error);
        }
      });
  }, []);

  useEffect(() => {
    if (response != "") {
      setProgressText("Training dataset, this may take a while...");
      if (response == "trainData") {
        axios.get("http://127.0.0.1:8000/train-dataset").then((results) => {
          if (results.data["response"] == "Ok") {
            setResponse("testModel");
          } else {
            setError(!error);
          }
        });
      } else {
        setProgressText("Testing model...");
        axios.get("http://127.0.0.1:8000/test-model").then((results) => {
          if (results.data["response"] == "Ok") {
            navigate("/results");
          } else {
            setError(!error);
          }
        });
      }
    }
  }, [response]);

  return (
    <>
      { error ? (
        <>
          <h1> An error has ocurred...</h1>
          <h2> Review the requirements of the project!</h2>
          <br />
          <i class="fa-solid fa-face-frown fa-fade fa-10x"></i>
        </>
        
      ) : (
        <>
          <h1> { progressText } </h1>
          <br />
          <i className="fa-solid fa-rocket fa-spin fa-10x"></i>
        </>
      )}
    </>
  );
};
