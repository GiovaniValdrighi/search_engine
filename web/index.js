


function query_link(text_query) {
  //const text_query = document.getElementById("text_query").value
  const URL = "http://localhost:8080/query?text="+text_query;

  fetch(URL)
    .then(function (response) {
        return response.json();
    })
    .then(function (data) {
        document.getElementById('output').innerHTML = data.res;
    })
}

function query() {
  const text_query = document.getElementById("text_query").value
  const URL = "http://localhost:8080/query?text="+text_query;

  fetch(URL)
    .then(function (response) {
        return response.json();
    })
    .then(function (data) {
        document.getElementById('output').innerHTML = data.res;
    })
}