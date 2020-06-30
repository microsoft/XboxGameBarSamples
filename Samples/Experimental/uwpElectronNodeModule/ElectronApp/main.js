// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const GbRpc = require('./build/Release/GbRpc');

let renderID;
let mainID;
let serverStarted;

function createWindows() 
{
  const mainWindow = BrowserWindowFactory(800, 600, true);
  const renderWindow = BrowserWindowFactory(800, 600, false);

  // and load the index.html and 'background' render.html of the app.
  mainWindow.loadFile('index.html')
  renderWindow.loadFile('render.html')

  mainID = mainWindow.id;
  renderID = renderWindow.id;
  serverStarted = false;

  mainWindow.on('close' , () => closeWindows())
}

function BrowserWindowFactory(windowWidth, windowHeight, showWindow)
{
  return new BrowserWindow({
    width: windowWidth,
    height: windowHeight,
    show: showWindow,
    webPreferences: {
      nodeIntegration: true
    }
  })
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(createWindows)

// Quit when all windows are closed.
app.on('window-all-closed', function () 
{
  // On OS X it is common for applications and their menu bar.
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') 
  {
    app.quit()
  }
})

app.on('activate', function () 
{
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) 
  {  
    createWindow()
  }
})

//  Callback for window closing cleanup work.
function closeWindows()
{
  let windows = BrowserWindow.getAllWindows();

  windows.forEach(element => 
  {
    if (element !== null && element.id !== mainID)
    {
      element.close();
    }
  });
}
 
function sendCommand(id)
{
  let window = BrowserWindow.fromId(id);
  
  if (window !== null)
  {
    window.webContents.send('StartServer', 'Start server command sent from main')
  }
  else
  {
    const newWindow = BrowserWindowFactory(800, 600, false);
    renderID = newWindow.id;
    newWindow.loadFile('render.html');
    newWindow.webContents.on("did-finish-load", () => newWindow.webContents.send('StartServer', 'Start server command sent from main'));
  }

  serverStarted = true;
}

ipcMain.on('StartServer', (event, arg) => 
{
  if (!serverStarted)
  {
    console.log(arg) 
    sendCommand(renderID);
  }
  else
  {
    console.log(arg) 
    console.log('RESPONSE: App is already connected');
    event.reply('asynchronous-reply', 'RESPONSE: App is already connected');
  }
})

ipcMain.on('CloseServer', (event, arg) => 
{
  console.log(arg)
  
  const window = BrowserWindow.fromId(renderID);
  if (window !== null)
  {
    if (serverStarted)
    {
      serverStarted = false;
      window.close();
      console.log("RESPONSE: App disconnected")
      event.reply('asynchronous-reply', 'RESPONSE: App disconnected.')
    }
    else
    {
      console.log("RESPONSE: App is already disconnected ")
      event.reply('asynchronous-reply', 'RESPONSE: App is already disconnected')
    }
  }
  else
  {
    console.error("RESPONSE: App is already disconnected")
    event.reply('asynchronous-reply', 'Warning: App is already disconnected')
  }
})

ipcMain.on('asynchronous-reply', (event, arg) => 
{
  console.log(arg)
  const window = BrowserWindow.fromId(mainID);
  window.webContents.send('asynchronous-message', arg)
})
