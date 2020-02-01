open ReactUtils;

[@react.component]
let make = () => {
  <div className="auth-page">
    <div className="container page">
      <div className="row">
        <div className="col-md-6 offset-md-3 col-xs-12">
          <h1 className="text-xs-center"> "Sign in"->s </h1>
          <p className="text-xs-center">
            <Link href="#/register"> "Need an account"->s </Link>
          </p>
          <LoginForm />
        </div>
      </div>
    </div>
  </div>;
};