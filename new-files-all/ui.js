let fieldRoom;
let ledstatus=0;
let doorstatus=0;

function readTemperature(apiKey) {
  var url = `https://api.thingspeak.com/channels/2163504/fields/1.json?api_key=${apiKey}&results=2`;

  fetch(url)
    .then(response => response.json())
    .then(data => {
      var temperature = parseFloat(data.feeds[1].field1);
      var temperatureElement = document.querySelector('#temperature');
      temperatureElement.innerHTML = temperature.toFixed(2)+"&deg;C";
    })
    .catch(error => {
      console.error('Error:', error);
    });
}


function readLPGDetectionStatus(apiKey) {
  var url = `https://api.thingspeak.com/channels/2163504/fields/1.json?api_key=${apiKey}&results=2`;

  fetch(url)
    .then(response => response.json())
    .then(data => {
      // var lpgAlert = document.getElementById("lpgAlert");
    var field1Value2 =data.feeds[1].field2;
      if (field1Value2 === '1') {
        // console.log('ifyes')
        const divid = document.getElementById('alerts');
        if(divid.innerHTML === "")
        {
          // console.log('yesif');
          // fireAlert.innerHTML = "Fire detected! Please evacuate.";
          // fireAlert.style.color = "red";
          const div = document.createElement('div');
          div.classList.add('alert');
          div.innerHTML = `<h2>ALERT!</h2>
          <p>LPG LEAK DETECTED! PLEASE EVACUATE.</p>
          `;
          document.getElementById('alerts').appendChild(div);
        }
      }
      else {
        const div = document.getElementById('alerts');
        const child = div.querySelector('.alert');
        child.remove();
        // div.appendChild(clas);
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
}

// smoke detection
function readFireDetectionStatus(apiKey) {
  var url = `https://api.thingspeak.com/channels/2165472/fields/1.json?api_key=${apiKey}&results=2`;
    // console.log('yes');
  fetch(url)
    .then(response => response.json())
    .then(data => {
    // var fireAlert = document.getElementById("fireAlert");
    // console.log('yes');
    // console.log(data);
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
          div.innerHTML = `<h2>ALERT!</h2>
          <p>HAZARD DETECTED! PLEASE EVACUATE.</p>
          `;
          document.getElementById('alerts1').appendChild(div);
        }
      }
      else {
        const div = document.getElementById('alerts1');
        const child = div.querySelector('.alert');
        child.remove();
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
    var url = `https://api.thingspeak.com/update?api_key=${apiKey}&field${fieldRoom}=${field1Value}`;
    console.log(fieldRoom);
    // console.log(field1Value);
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
    var channelID = '2151521'; // Replace with your ThingSpeak channel ID
    var url = `https://api.thingspeak.com/channels/${channelID}/fields/1-4.json?api_key=${apiKey}&results=10`;
  
    fetch(url)
      .then(response => response.json())
      .then(data => {
        var labels = [];
        var heartRateData = [];
        var oxygenLevelData = [];
  
        // Extract data from ThingSpeak response
        for (var i = 0; i < data.feeds.length; i++) {
          var feed = data.feeds[i];
          labels.push(feed.created_at);
          heartRateData.push((feed.field1));
          oxygenLevelData.push((feed.field4));
        }
        // console.log(feed.field7);
  
        // Create a chart
        var ctx = document.getElementById('chart_1').getContext('2d');
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

// Control functions
function toggleLight() {
    // console.log("Toggle Light function called");
  
    var lightButton = document.getElementById("lightButton");
    // console.log("Current innerHTML:", lightButton.innerHTML);

    var container = document.getElementById("lightpic");
    var originalBackground = container.style.backgroundImage;
      
    if (lightButton.innerHTML === "Turn Light On") {
        lightButton.innerHTML = "Turn Light Off";
        writeToThingSpeak('CQA08TED4R0B70XX', 1, fieldRoom);
        console.log("Turning light on");
        ledstatus=1;
        container.style.backgroundImage = "url('./pngs/onbulb.png')"; ///EDIT
    }
    else{
        lightButton.innerHTML = "Turn Light On";
        writeToThingSpeak('CQA08TED4R0B70XX', 0, fieldRoom);
        ledstatus=0;
        console.log("Turning light off");
        container.style.backgroundImage = "url('./pngs/offbulb.png')"; ///EDIT
    }

    // console.log("Updated innerHTML:", lightButton.innerHTML);

    jsondict={"doorstatus":doorstatus,"led":ledstatus}
    console.log(jsondict)
      fetch('http://localhost:8000/om2m', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(jsondict)
      
    })
    .then(response => {
      // Handle the response from the server
      console.log('Data sent successfully');
    })
    .catch(error => {
      // Handle any errors
      console.error('Error:', error);
    });
    
  }


  // Function to toggle door state
function toggleDoor() {
    var doorButton = document.getElementById("doorButton");
    var container = document.getElementById("lockpic");
  
    if (doorButton.innerHTML === "Open Door") {
      doorButton.innerHTML = "Close Door";
      doorstatus=1;
      writeToThingSpeak('UGEDUGRFKOC59M9R', 1, 1); // Write "1" to ThingSpeak to open the door
      console.log("Opening the door");
      container.style.backgroundImage = "url('./pngs/unlocked.png')"; ///EDIT

    } else {
      doorstatus=0;
      doorButton.innerHTML = "Open Door";
      writeToThingSpeak('UGEDUGRFKOC59M9R', 0, 1); // Write "0" to ThingSpeak to close the door
      console.log("Closing the door");
      container.style.backgroundImage = "url('./pngs/locked.png')"; ///EDIT

    }
    jsondict={"doorstatus":doorstatus,"led":ledstatus}
    console.log(jsondict)
      fetch('http://localhost:8000/om2m', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(jsondict)
      
    })
    .then(response => {
      // Handle the response from the server
      console.log('Data sent successfully');
    })
    .catch(error => {
      // Handle any errors
      console.error('Error:', error);
    });
  }
  
  // function acknowledgeAlert() {
  //   writeToThingSpeak('GFLOH9S1XRN3NPPS', 0, 3); // Write "0" to ThingSpeak to acknowledge the alert
  // }  

  // Example usage
  readAndDisplayData('2INB1VD6F20KFCG6');
  // readFireDetectionStatus('HZRO9QQ17NCBHIZF');
  setInterval(function() {
    readFireDetectionStatus('N62FEX47OUTCEIL4');
  }, 5000); // Read the smoke detection status every 5 seconds (adjust the interval as needed)

  setInterval(function() {
    readLPGDetectionStatus('DOIX278CD05M3IPD');
  }, 5000); // Read the smoke detection status every 5 seconds (adjust the interval as needed)
  setInterval(function() {
    readTemperature('25YTOHWLU63JIRZX');
  }, 5000);
  

  
  fetch('http://localhost:8000/endpoint')
  .then(response => response.json())
  .then(data => {
     if(data){
      console.log(data);
      const text=document.getElementById("position-text");
      text.innerHTML=`${data["data"]}`;
      fieldRoom=data["data"];
      }
      else{
        console.log("error")
      }
  })

  
  
