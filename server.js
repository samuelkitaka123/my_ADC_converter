const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const app = express();

// This line tells Node to "serve" your index.html from the public folder
app.use(express.static('public'));

app.get('/calculate', (req, res) => {
    // Get the inputs from the website
    const { type, freq, amp, fs, bits } = req.query;
    
    // Command to run your C++ .exe with the parameters
    // Note: Use "./engine.exe" on some systems
    const cmd = `engine.exe ${type} ${freq} ${amp} ${fs} ${bits}`;

    exec(cmd, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error}`);
            return res.status(500).send("C++ Engine Failed");
        }
        // Send the C++ output back to the website
        res.send(stdout);
    });
});

app.listen(3000, () => {
    console.log('Success! Open your browser to http://localhost:3000');
});