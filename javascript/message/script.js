const sendBtn = document.querySelector('#sendBtn');
const messageIn = document.querySelector('#messageIn');
const messageOut = document.querySelector('#messageOut');
const messageDelivered = document.querySelector('#messageDelivered');

sendBtn.addEventListener('click', sendMsg)

function sendMsg () {
  let content = messageIn.value;
  if(content === ''){
    alert('Please Enter a Valid Input')
  }else{
  messageDelivered.innerHTML = 'Last message delivered'
  messageOut.innerHTML = content;
  messageIn.value = '';
  }
}
