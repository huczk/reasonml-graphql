ReactDOMRe.renderToElementWithId(
  <ApolloHooks.Provider client=Client.client>
    <UserProvider> <App /> </UserProvider>
  </ApolloHooks.Provider>,
  "root",
);