function readLPGDetectionStatus(apiKey) {
  var url = `https://api.thingspeak.com/channels/2151464/fields/1.json?api_key=${apiKey}&results=2`;

  fetch(url)
    .then(response => response.json())
    .then(data => {
      var lpgAlert = document.getElementById("lpgAlert");
      if (data.status === 1) {
        lpgAlert.innerHTML = "LPG leak detected! Please evacuate.";
        lpgAlert.style.color = "red";
      } else {
        lpgAlert.innerHTML = "No LPG leak detected";
        lpgAlert.style.color = "black";
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
}

// smoke detection
function readFireDetectionStatus(apiKey) {
  var url = `https://api.thingspeak.com/channels/2151464/fields/1.json?api_key=${apiKey}&results=2`;
    console.log('yes');
  fetch(url)
    .then(response => response.json())
    .then(data => {
    // var fireAlert = document.getElementById("fireAlert");
    // console.log('yes');
    console.log(data);
    var field1Value1 =data.feeds[1].field1;
    console.log(field1Value1);
      if (field1Value1 === '1') {
        // console.log('ifyes')
        const divid = document.getElementById('alerts1');
        if(divid.innerHTML === "")
        {
          // console.log('yesif');
          // fireAlert.innerHTML = "Fire detected! Please evacuate.";
          // fireAlert.style.color = "red";
          const div = document.createElement('div');
          div.classList.add('alert');
          div.innerHTML = `<h2>Fire Alerts</h2>
          <p id="fireAlert" style="color: red;">Fire detected! Please evacuate.</p>
          <button onclick="acknowledgeAlert()">OK</button>
          `;
          document.getElementById('alerts1').appendChild(div);
        }
      }
      else {
        const div = document.getElementById('alerts1');
        const child = div.querySelector('.alert');
        div.removeChild(child);
        // div.appendChild(clas);
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
}


// Function to write data to ThingSpeak
function writeToThingSpeak(apiKey, field1Value, fieldNumber) {
    // var channelID = '2151464';
    var url = `https://api.thingspeak.com/update?api_key=${apiKey}&field${fieldNumber}=${field1Value}`;
    console.log(field1Value);
    fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
      }
    })
      .then(response => {
        if (response.ok) {
          console.log('Data written to ThingSpeak successfully');
        } else {
          console.error('Error writing data to ThingSpeak');
        }
      })
      .catch(error => {
        console.error('Error:', error);
      });
  }


//  Health Data
// Function to read data from ThingSpeak and display in a graph
function readAndDisplayData(apiKey) {
    var channelID = '2165791'; // Replace with your ThingSpeak channel ID
    var url = `https://api.thingspeak.com/channels/${channelID}/fields/1-4.json?api_key=${apiKey}&results=10`;
  
    fetch(url)
      .then(response => response.json())
      .then(data => {
        var labels = [];
        var heartRateData = [];
        var oxygenLevelData = [];
        var systolicPressureData = [];
        var diastolicPressureData = [];
  
        // Extract data from ThingSpeak response
        for (var i = 0; i < data.feeds.length; i++) {
          var feed = data.feeds[i];
          labels.push(feed.created_at);
          heartRateData.push((feed.field1));
          oxygenLevelData.push((feed.field4));
          systolicPressureData.push((feed.field7));
          diastolicPressureData.push((feed.field8));
        }
        // console.log(feed.field7);
  
        // Create a chart
        var ctx = document.getElementById('chart').getContext('2d');
        var chart = new Chart(ctx, {
          type: 'line',
          data: {
            labels: labels,
            datasets: [
              {
                label: 'Heart Rate',
                data: heartRateData,
                borderColor: 'red',
                fill: false,
              },
              {
                label: 'Oxygen Level',
                data: oxygenLevelData,
                borderColor: 'blue',
                fill: false,
              },
              {
                label: 'Systolic Pressure',
                data: systolicPressureData,
                borderColor: 'green',
                fill: false,
              },
              {
                label: 'Diastolic Pressure',
                data: diastolicPressureData,
                borderColor: 'orange',
                fill: false,
              },
            ],
          },
          options: {
            responsive: true,
            scales: {
              x: {
                display: true,
                title: {
                  display: true,
                  text: 'Time',
                },
              },
              y: {
                display: true,
                title: {
                  display: true,
                  text: 'Value',
                },
              },
            },
          },
        });
      })
      .catch(error => {
        console.error('Error:', error);
      });
  }
  

// Function to read data from ThingSpeak
function readFromThingSpeak(apiKey) {
  var channelID = '2151464';
  var url = `https://api.thingspeak.com/channels/${channelID}/fields/1.json?api_key=${apiKey}&results=2`;

  fetch(url)
    .then(response => response.json())
    .then(data => {
      var field1Value = data.feeds[0].field1;

      var smokeAlert = document.getElementById("smokeAlert");
      if (field1Value === "1") {
        smokeAlert.innerHTML = "Smoke detected! Please evacuate.";
        smokeAlert.style.color = "red";
      } else {
        smokeAlert.innerHTML = "No smoke detected";
        smokeAlert.style.color = "black";
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
}


// Control functions
function toggleLight() {
    // console.log("Toggle Light function called");
  
    var lightButton = document.getElementById("lightButton");
    // console.log("Current innerHTML:", lightButton.innerHTML);
  
    if (lightButton.innerHTML === "Turn Light On") {
        lightButton.innerHTML = "Turn Light Off";
        writeToThingSpeak('GFLOH9S1XRN3NPPS', 1, 1);
        console.log("Turning light on");
    }
    else{
        lightButton.innerHTML = "Turn Light On";
        writeToThingSpeak('GFLOH9S1XRN3NPPS', 0, 1);
        console.log("Turning light off");
    }

    // console.log("Updated innerHTML:", lightButton.innerHTML);
  }


  // Function to toggle door state
function toggleDoor() {
    var doorButton = document.getElementById("doorButton");
  
    if (doorButton.innerHTML === "Open Door") {
      doorButton.innerHTML = "Close Door";
      writeToThingSpeak('GFLOH9S1XRN3NPPS', 1, 2); // Write "1" to ThingSpeak to open the door
      console.log("Opening the door");
    } else {
      doorButton.innerHTML = "Open Door";
      writeToThingSpeak('GFLOH9S1XRN3NPPS', 0, 2); // Write "0" to ThingSpeak to close the door
      console.log("Closing the door");
    }
  }
  
  function acknowledgeAlert() {
    writeToThingSpeak('GFLOH9S1XRN3NPPS', 0, 3); // Write "0" to ThingSpeak to acknowledge the alert
  }  

  // Example usage
  readFromThingSpeak('HZRO9QQ17NCBHIZF');
  readAndDisplayData('7AUH6ES17NOR5GPL');
  // readFireDetectionStatus('HZRO9QQ17NCBHIZF');
  setInterval(function() {
    readFireDetectionStatus('HZRO9QQ17NCBHIZF');
  }, 5000); // Read the smoke detection status every 5 seconds (adjust the interval as needed)

  // setInterval(function() {
  //   readLPGDetectionStatus('YOUR_API_KEY');
  // }, 5000); // Read the smoke detection status every 5 seconds (adjust the interval as needed)
  
    fetch('http://localhost:8000/endpoint')
.then(response => response.json())
.then(data => {
    if(data){
    console.log(data);
    
    }
    else{
      console.log("error")
    }
})
