using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleGUI
{
    public  class CCommandParser
    {

        public bool TryParseCommandLine(string commandLine, out string outCommand, out Dictionary<string, string> outOptions, out string outErrorMessage)
        {
            outCommand = string.Empty;
            outOptions = new Dictionary<string, string>();
            outErrorMessage = string.Empty;

            var tokens = new List<string>();
            var currentToken = "";
            var insideQuotes = false;

            foreach (var ch in commandLine)
            {
                if (ch == '"')
                {
                    insideQuotes = !insideQuotes;
                    continue;
                }

                if (ch == ' ' && !insideQuotes)
                {
                    if (!string.IsNullOrEmpty(currentToken))
                    {
                        tokens.Add(currentToken);
                        currentToken = "";
                    }
                }
                else
                {
                    currentToken += ch;
                }
            }

            if (insideQuotes)
            {
                outErrorMessage = "Error: Unmatched quotes in command line.";
                return false;
            }

            if (!string.IsNullOrEmpty(currentToken))
            {
                tokens.Add(currentToken);
            }

            if (tokens.Count == 0)
            {
                outErrorMessage = "Error: Empty command line.";
                return false;
            }

            outCommand = tokens[0];
            tokens.RemoveAt(0);

            if (string.IsNullOrEmpty(outCommand))
            {
                outErrorMessage = "Error: Empty command.";
                return false;
            }

            for (int i = 0; i < tokens.Count; ++i)
            {
                if (tokens[i].StartsWith("--"))
                {
                    var key = tokens[i].Substring(2);
                    string value = string.Empty;

                    if (string.IsNullOrEmpty(key))
                    {
                        outErrorMessage = "Error: Empty key.";
                        return false;
                    }

                    if (i + 1 < tokens.Count && !tokens[i + 1].StartsWith("--"))
                    {
                        value = tokens[++i];
                    }

                    outOptions.Add(key, value);
                }
            }

            return true;
        }
    }
}

