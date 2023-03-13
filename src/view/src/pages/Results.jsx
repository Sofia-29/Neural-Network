import React, { useState, useEffect } from "react";
import axios from "axios";

import { LineChart } from "../components/LineChart";

export const Results = () => {
    const [data, setData] = useState([]);
    const [labels, setlabels] = useState([]);
    const [seconds, setSeconds] = useState("");
    const [loading, setLoading] = useState(true);

  useEffect(() => {
    axios.get("http://127.0.0.1:8000/results").then((results) => {
        console.log(results);
      if (results.data["response"] == "Ok") {
        setData(results.data["data"]);
        setlabels(results.data["labels"]);
        setSeconds(results.data["seconds"]);
        setLoading(false);
      }
    });
  }, []);

  return (
    <>
        { loading ? (
            <i className="fa-solid fa-rocket fa-spin fa-10x"></i>
            ) : (
                <>
                    <h1> Model trained in { seconds } seconds </h1>
                    <LineChart text={"Sum of squared errors in training data"} labels = { labels } data = { data }></LineChart>
                </>
            )   
        }

    </>
  )
};
