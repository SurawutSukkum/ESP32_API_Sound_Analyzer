import React, { useState, useEffect } from "react";
import { LineChart, Line, XAxis, YAxis, Tooltip, CartesianGrid, ResponsiveContainer } from "recharts";
import { createTheme, ThemeProvider, CssBaseline } from "@mui/material";

  // Dark theme configuration
  const darkTheme = createTheme({
    palette: {
      mode: "dark",
      primary: { main: "#FFD700" }, // Gold
      secondary: { main: "#C0A060" }, // Deeper Gold
      background: { default: "#00FF00", paper: "#000000" },
      text: { primary: "#F0FFFF", secondary: "#F0FFFF" },
      fontFamily: "Arial, sans-serif",
      
    },
    typography: {
      fontFamily: "Arial, sans-serif",
      h4: { color: "#E6E6FA" },
      h5: { color: "#E6E6FA" },
      h55: { color: "#FFFFFF" },
    },
  });

/*
const SoundGraph = () => {
  const [audioData, setAudioData] = useState([]);

  useEffect(() => {
    const generateSineWave = () => {
      const sampleRate = 100;
      const frequency = 5; // Hz
      const amplitude = 100;
      const dataLength = 1000;
      const newAudioData = Array.from({ length: dataLength }, (_, index) => ({
        time: index,
        amplitude: amplitude * Math.sin((2 * Math.PI * frequency * index) / sampleRate),
      }));
      setAudioData(newAudioData);
    };

    generateSineWave();
  }, []);
*/
const SoundGraph = () => {
  const [audioData, setAudioData] = useState([]);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch("http://127.0.0.1:3000/audio"); // Replace with actual API URL
        const rawData = await response.arrayBuffer();
        const int16Array = new Int16Array(rawData);

        const formattedData = Array.from(int16Array).map((value, index) => ({
          time: index,
          amplitude: value,
        }));

        setAudioData(formattedData);
      } catch (error) {
        console.error("Error fetching audio data:", error);
      }
    };

    fetchData();
  }, []);
  return (
    <ThemeProvider theme={darkTheme}>
     <CssBaseline /> {/* Ensures background stays dark */}
      <div className="p-4">
        <h2 className="text-xl font-bold mb-4"> เครื่องตรวจสมรรถภาพปอด  </h2>
        <ResponsiveContainer width="100%" height={300}>
        <LineChart data={audioData} style={{ backgroundColor: "black" }}>
          <XAxis dataKey="time" hide />
          <YAxis stroke="#ffffff" />
          <Tooltip contentStyle={{ backgroundColor: "#333", color: "#fff" }} />
          <CartesianGrid strokeDasharray="3 3" stroke="#555" />
          <Line type="monotone" dataKey="amplitude" stroke="#8884d8" dot={false} />
        </LineChart>
        </ResponsiveContainer>
      </div>
      </ThemeProvider>  
  );
};

export default SoundGraph;
