using System.Collections.Generic;
using System.IO.Ports;
using System.Collections.Concurrent;

public class PuzzleMaster {
    public const int NUM_READERS = 8;
    private int nextReaderId = 1; // 1-based sequence so "empty value" of 0 is easy to detect as an error
    private ConcurrentDictionary<string, int> readerMacs = new ConcurrentDictionary<string, int>();
    
    private string _portName;

    private List<TaggedObject> _readerState;
    private object _readerStateLock = new object();
    private bool _didWand = false;
    private PuzzleSolution _lastSolution = null;

    static private PuzzleMaster _instance = new PuzzleMaster();

    static PuzzleMaster() { }

    private PuzzleMaster() {
        _readerState = new List<TaggedObject>();

        // NOTE: string[] ports = SerialPort.GetPortNames(); to get port names if needed
        string[] ports = SerialPort.GetPortNames();
        SerialPort _serialPort = null;
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
                readerMacs.TryAdd(readerMac, readerId);
                lock (_readerStateLock) {
                    _readerState.Add(obj);
                }
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

        // Update puzzle tracking
        List<TaggedObject> stateCopy;
        TaggedObject  oldObj;
        lock (_readerStateLock) {
            oldObj = _readerState[readerId - 1];
            _readerState[readerId - 1] = obj;
            stateCopy = new List<TaggedObject>(_readerState);
        }

        // look for solution
        PuzzleSolution solution = _lastSolution;
        if (nextReaderId > 2 && oldObj != obj) {
            // Only look for a solution once at least two of the readers are initialized
            solution = PuzzleSolution.FindMatchingSolution(stateCopy);
            _lastSolution = solution;
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
                SerialPort _serialPort = new SerialPort();
                _serialPort.PortName = _portName;
                _serialPort.BaudRate = 9600;
                _serialPort.Open();
                _serialPort.WriteLine(solution.radioCommand);
                _serialPort.Close();
            }
        }
        
        // Return colors for object and a (potentially updated) readerId
        string response = "";
        if (flashMode == "2") {
            // Flash mode 2 just red<->white for all
            for (int k = 0; k < 6; ++k) {
                response += "FFFFFF";
            }
        }
        else {
            for (int k = 0; k < 6; ++k) {
                response += ((int)obj.colors[k]).ToString("X6");
            }
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