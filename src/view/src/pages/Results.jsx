import React, { useState, useEffect } from "react";
import axios from "axios";
import IconButton from "@mui/material/IconButton";
import Stack from "@mui/material/Stack";
import ArrowBackIosNewIcon from "@mui/icons-material/ArrowBackIosNew";
import ArrowForwardIosIcon from "@mui/icons-material/ArrowForwardIos";

import { LineChart } from "../components/LineChart";
import { BarChart } from "../components/BarChart";

const styles = {
  color: "rgba(255, 255, 255, 0.87)", // Cambia el color a rojo
};

export const Results = () => {
  const [traininData, setTraininData] = useState([]);
  const [labelsTrainingData, setLabelsTrainingData] = useState([]);
  const [validationData, setValidationData] = useState([]);
  const [labelsValidationData, setLabelsValidationData] = useState([]);
  const [validationErrors, setValidationErrors] = useState(0);
  const [testData, setTestData] = useState([]);
  const [labelsTestData, setLabelsTestData] = useState([]);
  const [testsErrors, setTestsErrors] = useState(0);
  const [resultDisplayed, setResultDisplayed] = useState(0);
  const [seconds, setSeconds] = useState("");
  const [userDecision, setUserDecision] = useState(true);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(false);

  useEffect(() => {
    axios.get("http://127.0.0.1:8000/results").then((results) => {
      if (results.data["response"] == "Ok") {
        setTraininData(results.data["trainingData"]);
        setLabelsTrainingData(results.data["trainingLabels"]);
        setSeconds(results.data["seconds"]);
        if (!results.data["userDecision"]) {
          setUserDecision(false);
        } else {
          setValidationData(results.data["validationData"]);
          setLabelsValidationData(results.data["validationLabels"]);
          setValidationErrors(results.data["validationErrors"]);
          setTestData(results.data["testData"]);
          setLabelsTestData(results.data["testLabels"]);
          setTestsErrors(results.data["testErrors"]);
          setUserDecision(true);
        }
        setLoading(false);
      } else {
        setError(!error);
      }
    });
  }, []);

  const handleIncrementOptionChange = () => {
    setResultDisplayed(resultDisplayed + 1);
  };

  const handleDecrementOptionChange = () => {
    setResultDisplayed(resultDisplayed - 1);
  };

  return (
    <>
      {error ? (
        <>
          <h1> An error has ocurred, review the requirements of the project! </h1>
          <br />
          <i class="fa-solid fa-face-frown fa-fade fa-10x"></i>
        </>
      ) : loading ? (
        <i className="fa-solid fa-rocket fa-spin fa-10x"></i>
      ) : (
        <>
          { resultDisplayed == 0 ? (
            <>
              <h1> Model trained in {seconds} seconds </h1>
              <LineChart
                text={"Sum of squared errors in training data each iteration"}
                labels={labelsTrainingData}
                data={traininData}
              ></LineChart>
            </>
          ) : resultDisplayed == 1 ? (
            <>
              <h1> Results of validation data </h1>
              <h2> Number of wrong predictions: {validationErrors} </h2>
              <BarChart
                text={"Model test results with validation data"}
                labels={labelsValidationData}
                datasetLabel={"Number of categories found "}
                data={validationData}
              />
            </>
          ) : (
            <>
              <h1> Results of test data </h1>
              <h2> Number of wrong predictions: {testsErrors} </h2>
              <BarChart
                text={"Model test results with test data"}
                labels={labelsTestData}
                datasetLabel={"Number of categories found "}
                data={testData}
              />
            </>
          )}

          { userDecision ? (
            <Stack direction="row" spacing={1}>
              <IconButton
                style={styles}
                size="large"
                disabled={resultDisplayed == 0 ? true : false}
                onClick={() => handleDecrementOptionChange()}
              >
                <ArrowBackIosNewIcon />
              </IconButton>
              <IconButton
                style={styles}
                size="large"
                disabled={resultDisplayed == 2 ? true : false}
                onClick={() => handleIncrementOptionChange()}
              >
                <ArrowForwardIosIcon />
              </IconButton>
            </Stack>
          ) : (
            <></>
          ) }
        </>
      )}
    </>
  );
};
