import { useEffect, useState } from "react";
import { redirect } from "react-router-dom";
import { Checkbox, FormGroup, FormControlLabel } from "@mui/material";
import { styled } from "@mui/material/styles";
import Button from "@mui/material/Button";
import Box from "@mui/material/Box";
import axios from "axios";


import "../styles/home.css";

const ColorButton = styled(Button)(({ theme }) => ({
  color: "#f2f2f2",
  backgroundColor: "#757575",
  "&:hover": {
    backgroundColor: "#757575",
  },
}));

export const Home = () => {
  const [normalizeData, setNormalizeData] = useState(true);
  const [mapOutDesiredOutput, setmapOutDesiredOutput] = useState(true);
  const [selectedFile, setSelectedFile] = useState(null);

  const handleFileSelect = (event) => {
    const file = event.target.files[0];
    if (file.type !== "text/csv") {
      console.log("Only CSV files are accepted");
      return;
    }
    setSelectedFile(file);
  };

  const handleChangeNormalizeData = () => {
    setNormalizeData(!normalizeData);
  };

  const handleChangeMapOutDesiredOutput = () => {
    setmapOutDesiredOutput(!mapOutDesiredOutput);
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
    const fileInput = document.getElementById('fileInput');
    fileInput.click();
  };

  useEffect( () => {
    console.log(selectedFile);
  }, [selectedFile]);

  const submitDataset = () => {
    const formData = new FormData();
    formData.append("file", selectedFile);
    formData.append("normalize_data", normalizeData);
    formData.append("map_desired_output", mapOutDesiredOutput);

    axios
      .post("http://127.0.0.1:8000/split-dataset", formData, {
        headers: {
          "Content-Type": "multipart/form-data",
        },
      })
      .then((response) => {
        console.log(response);
      });
  };

  return (
    <>
      <div
        className="drag-drop-area"
        onDrop = { handleDrop }
        onDragOver={(event) => event.preventDefault()}
        onDragEnter={(event) => event.preventDefault()}
        onClick={ handleSelectClick }
      >
          { selectedFile ? (
            <p>{ selectedFile.name }</p>
          ) : (
            <p>Drop CSV file here or click to select</p>
          ) }
          <input
            id="fileInput"
            type="file"
            accept=".csv"
            onChange={ handleFileSelect }
            style={{ display: 'none' }}
          />
      </div>

      <FormGroup row={true} sx={{ m: 3 }}>
        <FormControlLabel
          control={
            <Checkbox
              checked={normalizeData}
              sx={{
                color: "#f2f2f2",
                "&.Mui-checked": {
                  color: "#f2f2f2",
                },
              }}
              onClick={handleChangeNormalizeData}
            />
          }
          label={
            <Box component="div" fontSize={20}>
              Normalize data
            </Box>
          }
        />
        <FormControlLabel
          control={
            <Checkbox
              checked={mapOutDesiredOutput}
              sx={{
                color: "#f2f2f2",
                "&.Mui-checked": {
                  color: "#f2f2f2",
                },
              }}
              onClick={handleChangeMapOutDesiredOutput}
            />
          }
          label={
            <Box component="div" fontSize={20}>
              Map out labels
            </Box>
          }
        />
      </FormGroup>

      <ColorButton
        variant="contained"
        disabled={selectedFile ? false : true}
        onClick={() => {
          submitDataset();
          redirect("/progress-bar");
        }}
      >
        Submit
      </ColorButton>
    </>
  );
};
