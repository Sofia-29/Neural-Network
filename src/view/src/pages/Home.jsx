import { useState } from "react";
import { useNavigate } from 'react-router-dom';
import { Checkbox, FormGroup, FormControlLabel, 
  Button, Box, styled } from "@mui/material";


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
  const [selectedFile, setSelectedFile] = useState(null);
  const navigate = useNavigate();

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
            <Box component="div" fontSize={ 20 }>
              Map out labels
            </Box>
          }
        />
      </FormGroup>

      <ColorButton
        variant="contained"
        disabled={selectedFile ? false : true}
        onClick={() => {
          navigate('/dataset-processing',{ state:
            { normalizeData: normalizeData, mapOutDesiredOutput: mapOutDesiredOutput, file: selectedFile } }
          );
        }}
      >
        Submit
      </ColorButton>
    </>
  );
};
