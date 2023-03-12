import { Checkbox, FormControlLabel, Box } from "@mui/material";

export const CustomCheckbox = ( { checked, onClick, label }) => {
  return (
    <>
      <FormControlLabel control=
      {
        <Checkbox
          checked={ checked }
          sx={{
            color: "#f2f2f2",
            "&.Mui-checked": {
              color: "#f2f2f2",
            },
          }}
          onClick={ onClick }
        />
      }
      label=
      {
        <Box component="div" fontSize={ 20 }>
          { label }
        </Box>
      }
      />
    </>
  );
};
