using System.Collections.Generic;
using System.IO.Ports;

public class PuzzleMaster {
    public const int NUM_READERS = 8;
    private int nextReaderId = 1; // 1-based sequence so "empty value" of 0 is easy to detect as an error
    private Dictionary<string, int> readerMacs = new Dictionary<string, int>();
    
    private SerialPort _serialPort;
    private string _portName;

    private List<TaggedObject> _readerState;
    private bool _didWand = false;

    static private PuzzleMaster _instance = new PuzzleMaster();

    static PuzzleMaster() { }

    private PuzzleMaster() {
        _readerState = new List<TaggedObject>();

        // NOTE: string[] ports = SerialPort.GetPortNames(); to get port names if needed
        string[] ports = SerialPort.GetPortNames();
        _serialPort = null;
        foreach (string port in ports) {
            if (port.StartsWith("/dev/tty.usbmodem")) {
                _portName = port;
                _serialPort = new SerialPort();
                _serialPort.PortName = _portName;
                _serialPort.BaudRate = 9600;
                _serialPort.Open();
                _serialPort.Close();
                break;
            }
        }
        if (_serialPort == null) {
            throw new System.Exception("Where's my serial?");
        }
    }

    public string AssignObject(string readerMac, TaggedObject obj) {
        // Mac address already assigned to sequence?
        int readerId;
        if (!readerMacs.TryGetValue(readerMac, out readerId)) {
            // Assign next available reader ID
            if (nextReaderId >= 1 && nextReaderId <= NUM_READERS) {
                readerId = nextReaderId++;
                readerMacs.Add(readerMac, readerId);
                _readerState.Add(obj);
            }
            else {
                // Error for now
                return null;
            }
        }

        if (readerId < 1 || readerId > NUM_READERS) {
            // Error because we don't recognize this readerId
            return null;
        }
        
        // No solution found, by default
        string flashMode = "0";

        // Update puzzle tracking, look for solution
        TaggedObject oldObj = _readerState[readerId - 1];
        _readerState[readerId - 1] = obj;
        PuzzleSolution solution = null;
        if (nextReaderId > 2) {
            // Only look for a solution once at least two of the readers are initialized
            solution = PuzzleSolution.FindMatchingSolution(_readerState);
        }
        if (solution != null) {
            flashMode = solution.flashMode;

            // Only radio if a change was made
            if (oldObj != obj) {
                if (solution.radioCommand == "x") {
                    // Wand first, then amulet
                    // Store back into solution so if user repeats puzzle they get the same result
                    if (_didWand) {
                        solution.radioCommand = "a";
                    }
                    else {
                        solution.radioCommand = "w";
                        _didWand = true;
                    }
                }

                // DO A THING
                _serialPort = new SerialPort();
                _serialPort.PortName = _portName;
                _serialPort.BaudRate = 9600;
                _serialPort.Open();
                _serialPort.WriteLine(solution.radioCommand);
                _serialPort.Close();
            }
        }
        
        // Return colors for object and a (potentially updated) readerId
        string response = "";
        for (int k = 0; k < 6; ++k) {
            response += ((int)obj.colors[k]).ToString("X6");
        }
        response += flashMode;
        response += readerId.ToString("X2");
        return response;
    }

    static public PuzzleMaster Instance {
        get {
            return _instance;
        }
    }
}