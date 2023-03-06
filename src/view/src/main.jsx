import React from "react";
import ReactDOM from "react-dom/client";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";

import { Home } from "./pages/Home";
import { DatasetProcessing } from "./pages/DatasetProcessing";
import "./styles/main.css";

ReactDOM.createRoot(document.getElementById("root")).render(
  <React.StrictMode>
    <Router>
      <Routes>
        <Route exact path="/" element={<Home />} />
        <Route exact path="/dataset-processing" element={<DatasetProcessing />} />
      </Routes>
    </Router>
  </React.StrictMode>
);
