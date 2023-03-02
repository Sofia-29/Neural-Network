import { useState } from "react";
import { useFilePicker } from "use-file-picker";
import { Checkbox, FormGroup, FormControlLabel } from "@mui/material";
import { styled } from "@mui/material/styles";
import { FiUpload } from "react-icons/fi";

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

  const [openFileSelector, { filesContent, loading }] = useFilePicker({
    accept: ".csv",
    multiple: false,
  });

  const handleChangeNormalizeData = () => {
    setNormalizeData(!normalizeData);
  };

  const handleChangeMapOutDesiredOutput = () => {
    setmapOutDesiredOutput(!mapOutDesiredOutput);
  };

  const submitDataset = (dataset) => {
    let response = undefined;
    const file = new File([dataset], "archivo.csv", { type: "text/csv" });
    const formData = new FormData();
    formData.append('dataset', file);
    formData.append('normalize_data', normalizeData);
    formData.append('map_desired_output', mapOutDesiredOutput);

    axios.post("http://127.0.0.1:8000/split-dataset", formData, {
        headers: {
          "Content-Type": "multipart/form-data",
        },
      })
      .then((response) => {
        console.log(response);
      });

    return response;
  };

  return (
    <>
      {loading ? (
        <>
          <div>Loading...</div>
        </>
      ) : (
        <>
          <button className="upload-button" onClick={() => openFileSelector()}>
            {filesContent.length != 0 ? filesContent[0].name : "Select file"}
            <FiUpload></FiUpload>
          </button>

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
                  onClick={ handleChangeMapOutDesiredOutput }
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
            disabled={ filesContent.length != 0 ? false : true }
            onClick= { () => { 
                submitDataset(filesContent[0].content); 
              } 
            }
          >
            Submit
          </ColorButton>
        </>
      )}
    </>
  );
};
