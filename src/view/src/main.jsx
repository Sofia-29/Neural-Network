import React from "react";
import ReactDOM from "react-dom/client";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";

import { Home } from "./pages/Home";
import { DatasetProcessing } from "./pages/DatasetProcessing";
import { Results } from "./pages/Results";
import "./styles/main.css";

ReactDOM.createRoot(document.getElementById("root")).render(
  <React.StrictMode>
    <Router>
      <Routes>
        <Route exact path="/" element={ <Home /> } />
        <Route exact path="/dataset-processing" element={ <DatasetProcessing /> } />
        <Route exact path="/results" element={ <Results /> } />
      </Routes>
    </Router>
  </React.StrictMode>
);
