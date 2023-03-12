import { useState } from "react";
import { useNavigate } from "react-router-dom";
import { FormGroup, Button, Box, styled } from "@mui/material";

import { CustomCheckbox } from "../components/CustomCheckbox";
import "../styles/home.css";

const ColorButton = styled(Button)(() => ({
  color: "#f2f2f2",
  backgroundColor: "#757575",
  "&:hover": {
    backgroundColor: "#757575",
  },
}));

export const Home = () => {
  const [normalizeData, setNormalizeData] = useState(true);
  const [mapOutDesiredOutput, setmapOutDesiredOutput] = useState(true);
  const [splitDataset, setSplitDataset] = useState(true);
  const [selectedFile, setSelectedFile] = useState(null);
  const navigate = useNavigate();

  const handleChangeNormalizeData = () => {
    setNormalizeData(!normalizeData);
  };

  const handleChangeMapOutDesiredOutput = () => {
    setmapOutDesiredOutput(!mapOutDesiredOutput);
  };

  const handleChangeSplitDataset = () => {
    setSplitDataset(!splitDataset);
  };
  
  const handleFileSelect = (event) => {
    const file = event.target.files[0];
    if (file.type !== "text/csv") {
      console.log("Only CSV files are accepted");
      return;
    }
    setSelectedFile(file);
  };

  const handleDrop = (event) => {
    event.preventDefault();
    const file = event.dataTransfer.files[0];
    if (file.type !== "text/csv") {
      alert("Please drop a CSV file");
      return;
    }
    setSelectedFile(file);
  };

  const handleSelectClick = () => {
    const fileInput = document.getElementById("fileInput");
    fileInput.click();
  };

  return (
    <>
      <div
        className="drag-drop-area"
        onDrop={handleDrop}
        onDragOver={(event) => event.preventDefault()}
        onDragEnter={(event) => event.preventDefault()}
        onClick={handleSelectClick}
      >
        {selectedFile ? (
          <p>{selectedFile.name}</p>
        ) : (
          <p>Drop CSV file here or click to select</p>
        )}
        <input
          id="fileInput"
          type="file"
          accept=".csv"
          onChange={handleFileSelect}
          style={{ display: "none" }}
        />
      </div>

      <FormGroup row={true} sx={{ m: 3 }}>
        <CustomCheckbox
          checked={ normalizeData }
          onClick={ handleChangeNormalizeData }
          label={ "Normalize data" }
        ></CustomCheckbox>
        <CustomCheckbox
          checked={ mapOutDesiredOutput }
          onClick={ handleChangeMapOutDesiredOutput }
          label={"Map out labels"}
        ></CustomCheckbox>
        <CustomCheckbox
          checked={ splitDataset }
          onClick={ handleChangeSplitDataset }
          label={ "Split dataset" }
        ></CustomCheckbox>
      </FormGroup>

      <ColorButton
        variant="contained"
        disabled={ selectedFile ? false : true }
        onClick={() => {
          navigate("/dataset-processing", {
            state: {
              normalizeData: normalizeData,
              mapOutDesiredOutput: mapOutDesiredOutput,
              file: selectedFile,
              splitDataset: splitDataset,
            },
          });
        }}
      >
        Submit
      </ColorButton>
    </>
  );
};
