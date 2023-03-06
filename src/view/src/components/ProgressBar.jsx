import React from "react";
import { CircularProgressbar, buildStyles } from "react-circular-progressbar";
import "react-circular-progressbar/dist/styles.css";

import "../styles/dataset-processing.css";

export const ProgressBar = ({ value }) => {
  return (
    <>
      <div className="progress-bar">
        <CircularProgressbar
          value={value}
          text={`${value}%`}
          styles={buildStyles({
            textColor: "#f2f2f2",
            pathColor: "#757575",
            trailColor: "#f2f2f2",
          })}
        />
      </div>
    </>
  );
};
