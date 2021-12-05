function t = readtable(filename,varargin)
%READTABLE Create a table by reading from a file.
%
%   Use the READTABLE function to create a table by reading column-oriented
%   data from a file. READTABLE automatically determines the file format from
%   its extension as described below.
%
%   T = READTABLE(FILENAME) creates a table by reading from a file, where
%   FILENAME can be one of these:
%
%       - For local files, FILENAME can be a full path that contains
%         a filename and file extension. FILENAME can also be a relative path
%         to the current folder, or to a folder on the MATLAB path.
%         For example, to import a file on the MATLAB path:
%
%            T = readtable("patients.xls");
%
%       - For files from an Internet URL or stored at a remote location,
%         FILENAME must be a full path using a Uniform Resource Locator
%         (URL). For example, to import a remote file from Amazon S3,
%         specify the full URL for the file:
%
%            T = readtable("s3://bucketname/path_to_file/my_table.xls");
%
%         For more information on accessing remote data, see "Work with
%         Remote Data" in the documentation.
%
%   T = READTABLE(FILENAME,"FileType",FILETYPE) specifies the file type, where
%   FILETYPE is one of "text", "delimitedtext", "fixedwidth", "spreadsheet", or
%   "xml".
%
%   T = READTABLE(FILENAME,OPTS) creates a table by reading from a file stored
%   at FILENAME using the supplied ImportOptions OPTS. OPTS specifies variable
%   names, selected variable names, variable types, and other information regarding
%   the location of the data.
%
%   For example, import a subset of the data in a file:
%
%       opts = detectImportOptions("patients.xls");
%       opts.SelectedVariableNames = ["Systolic","Diastolic"];
%       T = readtable("patients.xls",opts)
%
%   READTABLE reads data from different file types as follows:
%
%   Text files (delimited and fixed-width): 
%
%     The following extensions are supported: .txt, .dat, .csv, .log,
%                                             .text, .dlm
%
%     Reading from a delimited text file creates one variable in T for each
%     column in the file. Variable names can be taken from the first row of
%     the file. By default, the variables created are either double, if the
%     column is primarily numeric, or datetime, duration, or text etc. If
%     data in a column cannot be converted to numeric, datetime or
%     duration, the column is imported as text.
%
%   Spreadsheet files:
%
%     The following extensions are supported: .xls, .xlsx, .xlsb, .xlsm, 
%                                             .xltm, .xltx, .ods
%
%     Reading from a spreadsheet file creates one variable in T for each
%     column in the file. By default, the variables created are either
%     double, datetime or text--depending on the type in the file.
%
%     READTABLE converts both empty fields or cells and values which cannot
%     be converted to the expected type to:
%       - NaN (for a numeric or duration variable),
%       - NaT (for a datetime variable),
%       - Empty character vector ('') or missing string (for text variables).
%
%   XML files:
%
%     The following extensions are supported: .xml
%
%     Tabular structure present within an XML file:
%
%         <table> ----------------------------- Table Node
%             <row> --------------------------- Row Node
%                 <date>2019-07-11</date> ----- Variable Node
%                 <index>8191</index>
%                 <name>Lorem</name>
%             </row>
%             <row>
%                 <date>2020-01-04</date>
%                 <index>131071</index>
%                 <name>Ipsum</name>
%             </row>
%         </table>
%
%     Reading from an XML file creates one row in T for each repeated node
%     in the file that is detected under the table node. Variable names are
%     taken from the names of the child nodes under the row nodes in the file.
%
%   Name-Value Pairs for ALL file types:
%   ------------------------------------
%
%   "FileType"              - Specify the file as "text", "delimitedtext",
%                             "fixedwidth", "spreadsheet", or "xml".
%
%   "VariableNamingRule"    - A character vector or a string scalar that
%                             specifies how the output variables are named.
%                             It can have either of the following values:
%
%                             'modify'   Modify variable names to make them
%                                        valid MATLAB Identifiers.
%                                        (default)
%                             'preserve' Preserve original variable names
%                                        allowing names with spaces and
%                                        non-ASCII characters.
%
%   "ReadVariableNames"     - Whether or not to expect variable names in
%                             the file.
%
%   "ReadRowNames"          - Whether or not to import the first column as
%                             row names.
%
%   "TreatAsMissing"        - Text which is used in a file to represent
%                             missing data, e.g. "NA".
%
%   "TextType"              - The type to use for text variables, specified
%                             as "char" or "string".
%
%   "DatetimeType"          - The type to use for date variables, specified
%                             as "datetime", "text", or "exceldatenum".
%                             Note: "exceldatenum" is not allowed for text.
%
%   Name-Value Pairs for TEXT and SPREADSHEET only:
%   -----------------------------------------------
%
%   "Range"                 - The range to consider when detecting data.
%                             Specified using any of the following syntaxes:
%                             - start cell: A string or character vector
%                               containing a column letter and a row number,
%                               or a 2 element numeric vector indicating
%                               the starting row and column.
%                             - full range: A start and end cell separated
%                               by colon, e.g. "C2:N15", or a four element
%                               numeric vector containing start row, start
%                               column, end row, end column, e.g. [2 3 15 13].
%                             - row range: A string or character vector
%                               containing a starting row number and ending
%                               row number, separated by a colon.
%                             - column range: A string or character vector
%                               containing a starting column letter and
%                               ending column letter, separated by a colon.
%                             - row number: A numeric scalar indicating the
%                               first row where data is found.
%
%   "NumHeaderLines"        - The number of header lines in the file.
%
%   "ExpectedNumVariables"  - The expected number of variables.
%
%   Name-Value Pairs for TEXT and XML only:
%   ---------------------------------------
%
%   "Encoding"           - The character encoding scheme associated with
%                          the file.  It is generally not necessary to specify
%                          the encoding as READTABLE will attempt to
%                          automatically determine the encoding.
%                          See the documentation for FOPEN for more information 
%
%   "DateLocale"         - The locale used to interpret month and day
%                          names in datetime text. Must be a character
%                          vector or scalar string in the form xx_YY.
%                          See the documentation for DATETIME for more
%                          information.
%
%   "DurationType"       - The type to use for duration, specified as
%                          "duration" or "text".
%
%   "Whitespace"         - Characters to treat as whitespace.
%
%   "DecimalSeparator"   - Character used to separate the integer part
%                          of a number from the decimal part of the
%                          number.
%
%   "ThousandsSeparator" - Character used to separate the thousands
%                          place digits.
%
%   "TrimNonNumeric"     - Logical used to specify that prefixes and
%                          suffixes must be removed leaving only the
%                          numeric data.
%
%   "HexType"            - Set the output type of a hexadecimal
%                          variable.
%
%   "BinaryType"         - Set the output type of a binary variable.
%
%   Name-Value Pairs for TEXT only:
%   -------------------------------
%
%   "Delimiter"                 - The delimiter(s) to use in the file.
%
%   "CommentStyle"              - The style of comments in the file.
%
%   "LineEnding"                - The line ending for the file.
%
%   "ConsecutiveDelimitersRule" - Behaviors to apply to fields containing
%                                 multiple consecutive delimiters:
%                                 - "split" - split consecutive delimiters
%                                             into multiple fields.
%                                 - "join"  - consecutive delimiters are
%                                             treated as a single delimiter.
%                                 - "error" - Ignored during detection
%                                             (treated as "split"), but the
%                                             resulting read will error.
%
%   "LeadingDelimitersRule"     - Behaviors to apply to delimiters at the
%                                 beginning of a line:
%                                 - "keep"   - keep the delimiter
%                                 - "ignore" - ignore the delimiter
%                                 - "error"  - return an error
%
%   Name-Value Pairs for SPREADSHEET only:
%   --------------------------------------
%
%   "Sheet"    - The sheet from which to detect the timetable.
%
%   "UseExcel" - A logical value that specifies whether or not to read the
%                spreadsheet file using Microsoft(R) Excel(R) on
%                Windows(R). Set "UseExcel" to one of these values:
%                - false - Does not open an instance of Microsoft Excel
%                          to read the file. This is the default setting.
%                          This setting may cause the data to be written
%                          differently for files with live updates (e.g.
%                          formula evaluation or plugins).
%                - true  - Opens an instance of Microsoft Excel to read
%                          the file on a Windows system with Excel installed.
%
%   Name-Value Pairs for XML only:
%   ------------------------------
%
%   "RowNodeName"                  - Node name which delineates rows of
%                                    the output table.
%
%   "RowSelector"                  - XPath expression that selects the XML
%                                    Element nodes which delineate rows of
%                                    the output table.
%
%   "VariableNodeNames"            - Node names which will be treated as 
%                                    variables of the output table.
%
%   "VariableSelectors"            - XPath expressions that select the XML
%                                    Element nodes to be treated as variables
%                                    of the output table.
%
%   "TableNodeName"                - Name of the node which contains table 
%                                    data. If multiple nodes have the same 
%                                    name, READTABLE uses the first node 
%                                    with that name.
%
%   "TableSelector"                - XPath expression that selects the XML
%                                    Element node containing the table data.
%
%   "VariableUnitsSelector"        - XPath expression that selects the  XML
%                                    Element nodes containing the variable units.
%
%   "VariableDescriptionsSelector" - XPath expression that selects the XML
%                                    Element  nodes containing the variable
%                                    descriptions.
%
%   "RowNamesSelector"             - XPath expression that selects the XML Element
%                                    nodes containing the row names.
%
%   "RepeatedNodeRule"             - Rule for managing repeated nodes in a 
%                                    given row of a table. Defaults to "addcol".
%
%   "ImportAttributes"             - Import XML node attributes as variables
%                                    of the output table. Defaults to true.
%
%   "AttributeSuffix"              - Suffix to append to all output table variable
%                                    names corresponding to attributes in the XML
%                                    file. Defaults to "Attribute".
%
%   "RegisteredNamespaces"         - The namespace prefixes that are mapped to
%                                    namespace URLs for use in selector expressions.
%
%   Name-Value Pairs supported with Text and Spreadsheet Import Options OPTS:
%   -------------------------------------------------------------------------
%
%   These have slightly different behavior when used with import options:
%
%       T = readtable(FILENAME, OPTS, "Name1", Value1, "Name2", Value2, ...)
%
%         "ReadVariableNames" true  - Reads the variable names from the
%                                     opts.VariableNamesRange or opts.VariableNamesLine
%                                     location.
%                             false - Uses variable names from the import options.
%
%         "ReadRowNames"      true  - Reads the row names from the opts.RowNamesRange
%                                     or opts.RowNamesColumn location.
%                             false - Does not import row names.
%
%       Text parameters:
%         "DateLocale" - Override the locale used when importing dates.
%         "Encoding"   - Override the encoding defined in import options.
%
%       Spreadsheet only parameters:
%         "Sheet"    - Override the sheet value in the import options.
%
%   See also READTIMETABLE, READMATRIX, READCELL, WRITETABLE, TABLE,
%            DETECTIMPORTOPTIONS, TEXTSCAN.

%   Copyright 2012-2020 The MathWorks, Inc.

[varargin{1:2:end}] = convertStringsToChars(varargin{1:2:end});
names = varargin(1:2:end);
try
    if any(strcmpi(names,"Format"))
        t = matlab.io.internal.legacyReadtable(filename,varargin);
    else
        func = matlab.io.internal.functions.FunctionStore.getFunctionByName('readtable');
        C = onCleanup(@()func.WorkSheet.clear());
        t = func.validateAndExecute(filename,varargin{:});
    end
catch ME
    throw(ME)
end
