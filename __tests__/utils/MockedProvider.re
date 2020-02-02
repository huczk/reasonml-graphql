type mock('a) = {
  request,
  result: result('a),
}
and request = {query: ReasonApolloTypes.queryString}
and result('a) = {data: 'a};

[@bs.module "@apollo/react-testing"] [@react.component]
external make:
  (~mocks: array(mock('a)), ~children: React.element) => React.element =
  "MockedProvider";